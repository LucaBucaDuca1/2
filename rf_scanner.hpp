/*
 * Copyright (C) 2024
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __RF_SCANNER_H__
#define __RF_SCANNER_H__

#include "ui.hpp"
#include "ui_widget.hpp"
#include "ui_navigation.hpp"
#include "ui_receiver.hpp"
#include "ui_freq_field.hpp"
#include "ui_rssi.hpp"
#include "event_m0.hpp"
#include "message.hpp"
#include "rf_path.hpp"
#include "max2837.hpp"
#include "volume.hpp"
#include "audio.hpp"
#include "receiver_model.hpp"
#include "app_settings.hpp"
#include "radio_state.hpp"
#include "portapack.hpp"
#include "string_format.hpp"
#include "file.hpp"
#include "freq_database.hpp"

#include <cstring>
#include <stdio.h>
#include <algorithm>

namespace ui {

// Detection entry for identified signals
struct DetectedSignal {
    uint32_t frequency;
    int16_t rssi;
    uint32_t timestamp;
    const char* device_type;
    const char* description;
    uint8_t confidence;
    bool is_active;
};

// Signal history for tracking
struct SignalHistory {
    uint32_t frequency;
    int16_t rssi_history[10];
    uint8_t history_index;
    uint32_t last_seen;
    uint32_t detection_count;
};

class RFScannerView : public View {
   public:
    RFScannerView(NavigationView& nav);
    ~RFScannerView();

    void focus() override;

    std::string title() const override { return "RF Device Scanner"; };

   private:
    NavigationView& nav_;

    // Scanning parameters
    uint32_t scan_start_freq = 150000000;  // 150 MHz (VHF Low)
    uint32_t scan_end_freq = 900000000;    // 900 MHz (Upper UHF)
    uint32_t scan_step = 25000;            // 25 kHz steps
    uint32_t current_freq = 150000000;

    // Detection settings
    int16_t rssi_threshold = -80;          // Minimum RSSI for detection
    uint16_t dwell_time_ms = 50;           // Time to spend on each frequency
    uint8_t min_detections = 3;            // Minimum detections for confidence

    // State
    bool scanning = false;
    bool found_signal = false;
    uint32_t scan_count = 0;
    uint32_t detections_count = 0;

    // Detected signals storage
    static constexpr size_t MAX_DETECTED = 50;
    DetectedSignal detected_signals[MAX_DETECTED];
    size_t detected_count = 0;

    // Signal history for analysis
    static constexpr size_t MAX_HISTORY = 100;
    SignalHistory signal_history[MAX_HISTORY];
    size_t history_count = 0;

    // Scan bands
    enum ScanBand {
        BAND_VHF_LOW,      // 150-174 MHz (Public Safety, Business)
        BAND_VHF_HIGH,     // 174-216 MHz (Government)
        BAND_UHF_T,        // 450-470 MHz (Public Safety)
        BAND_UHF_PUBLIC,   // 470-512 MHz (Government)
        BAND_700_MHZ,      // 700-800 MHz (LTE Public Safety)
        BAND_800_MHZ,      // 806-869 MHz (Public Safety Trunked)
        BAND_900_MHZ,      // 896-960 MHz (Cellular/Trunked)
        BAND_FULL          // Full scan
    };
    ScanBand current_band = BAND_FULL;

    // UI Elements
    Labels labels{
        {{0 * 8, 1 * 16}, "RF Device Scanner", Color::light_grey()},
        {{0 * 8, 2 * 16}, "Frequency:", Color::light_grey()},
        {{0 * 8, 4 * 16}, "RSSI:", Color::light_grey()},
        {{0 * 8, 6 * 16}, "Detections:", Color::light_grey()},
        {{0 * 8, 8 * 16}, "Last Device:", Color::light_grey()},
        {{0 * 8, 12 * 16}, "Band:", Color::light_grey()}
    };

    Text text_frequency{
        {10 * 8, 2 * 16, 20 * 8, 16},
        "---"};

    Text text_rssi{
        {6 * 8, 4 * 16, 15 * 8, 16},
        "---"};

    RSSI rssi{
        {0 * 8, 5 * 8, 240, 8}};

    Text text_detections{
        {12 * 8, 6 * 16, 18 * 8, 16},
        "0"};

    Text text_last_device{
        {0 * 8, 9 * 16, 30 * 8, 16},
        "---"};

    Text text_description{
        {0 * 8, 10 * 16, 30 * 8, 16},
        ""};

    Text text_band{
        {6 * 8, 12 * 16, 24 * 8, 16},
        "Full Scan"};

    OptionsField field_band{
        {0 * 8, 13 * 16},
        12,
        {{"VHF Low", BAND_VHF_LOW},
         {"VHF High", BAND_VHF_HIGH},
         {"UHF-T", BAND_UHF_T},
         {"UHF Public", BAND_UHF_PUBLIC},
         {"700 MHz", BAND_700_MHZ},
         {"800 MHz", BAND_800_MHZ},
         {"900 MHz", BAND_900_MHZ},
         {"Full Scan", BAND_FULL}}};

    NumberField field_threshold{
        {16 * 8, 13 * 16},
        3,
        {-120, -30},
        1,
        ' '};

    Button button_scan{
        {0 * 8, 15 * 16, 12 * 8, 3 * 16},
        "START"};

    Button button_clear{
        {13 * 8, 15 * 16, 12 * 8, 3 * 16},
        "CLEAR"};

    Button button_save{
        {26 * 8, 15 * 16, 10 * 8, 3 * 16},
        "SAVE"};

    Console console{
        {0, 19 * 8, 240, 11 * 8}};

    MessageHandlerRegistration message_handler_spectrum_config{
        Message::ID::ChannelSpectrumConfig,
        [this](const Message* const p) {
            (void)p;
        }};

    MessageHandlerRegistration message_handler_frame_sync{
        Message::ID::DisplayFrameSync,
        [this](const Message* const p) {
            (void)p;
            if (scanning) {
                this->on_tick();
            }
        }};

    void on_scan_toggle();
    void on_clear();
    void on_save();
    void on_tick();
    void on_band_changed(size_t index, int32_t value);
    void on_threshold_changed(int32_t value);

    void start_scan();
    void stop_scan();
    void process_signal(uint32_t freq, int16_t rssi);
    void identify_signal(uint32_t freq, int16_t rssi);
    void add_detection(uint32_t freq, const char* device, const char* desc, uint8_t confidence);
    void update_signal_history(uint32_t freq, int16_t rssi);
    void update_ui();
    void set_scan_range();
    void save_detections();
};

} /* namespace ui */

#endif /*__RF_SCANNER_H__*/
