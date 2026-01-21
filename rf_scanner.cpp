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

#include "rf_scanner.hpp"
#include "portapack.hpp"
#include "baseband_api.hpp"
#include "string_format.hpp"
#include "audio.hpp"
#include "rtc_time.hpp"
#include "file.hpp"
#include "freq_database.hpp"

using namespace portapack;

namespace ui {

RFScannerView::RFScannerView(NavigationView& nav)
    : nav_(nav) {

    add_children({&labels,
                  &text_frequency,
                  &text_rssi,
                  &rssi,
                  &text_detections,
                  &text_last_device,
                  &text_description,
                  &text_band,
                  &field_band,
                  &field_threshold,
                  &button_scan,
                  &button_clear,
                  &button_save,
                  &console});

    // Initialize detected signals
    for (size_t i = 0; i < MAX_DETECTED; i++) {
        detected_signals[i].is_active = false;
        detected_signals[i].confidence = 0;
    }

    // Initialize signal history
    for (size_t i = 0; i < MAX_HISTORY; i++) {
        signal_history[i].frequency = 0;
        signal_history[i].detection_count = 0;
        signal_history[i].history_index = 0;
    }

    // Set initial threshold
    field_threshold.set_value(rssi_threshold);

    // Button handlers
    button_scan.on_select = [this](Button&) {
        on_scan_toggle();
    };

    button_clear.on_select = [this](Button&) {
        on_clear();
    };

    button_save.on_select = [this](Button&) {
        on_save();
    };

    // Band selection handler
    field_band.on_change = [this](size_t index, int32_t value) {
        on_band_changed(index, value);
    };

    // Threshold handler
    field_threshold.on_change = [this](int32_t value) {
        on_threshold_changed(value);
    };

    console.writeln("RF Device Scanner Ready");
    console.writeln("Select band and press START");
}

RFScannerView::~RFScannerView() {
    if (scanning) {
        stop_scan();
    }
}

void RFScannerView::focus() {
    button_scan.focus();
}

void RFScannerView::on_scan_toggle() {
    if (scanning) {
        stop_scan();
    } else {
        start_scan();
    }
}

void RFScannerView::on_clear() {
    detected_count = 0;
    detections_count = 0;
    history_count = 0;

    for (size_t i = 0; i < MAX_DETECTED; i++) {
        detected_signals[i].is_active = false;
    }

    for (size_t i = 0; i < MAX_HISTORY; i++) {
        signal_history[i].frequency = 0;
        signal_history[i].detection_count = 0;
    }

    console.clear(true);
    console.writeln("Cleared all detections");
    text_detections.set("0");
    text_last_device.set("---");
    text_description.set("");
}

void RFScannerView::on_save() {
    save_detections();
}

void RFScannerView::on_band_changed(size_t index, int32_t value) {
    (void)index;
    current_band = static_cast<ScanBand>(value);
    set_scan_range();
}

void RFScannerView::on_threshold_changed(int32_t value) {
    rssi_threshold = value;
}

void RFScannerView::set_scan_range() {
    switch (current_band) {
        case BAND_VHF_LOW:
            scan_start_freq = 150000000;
            scan_end_freq = 174000000;
            scan_step = 12500;
            text_band.set("VHF Low (150-174 MHz)");
            break;
        case BAND_VHF_HIGH:
            scan_start_freq = 174000000;
            scan_end_freq = 216000000;
            scan_step = 25000;
            text_band.set("VHF High (174-216 MHz)");
            break;
        case BAND_UHF_T:
            scan_start_freq = 450000000;
            scan_end_freq = 470000000;
            scan_step = 12500;
            text_band.set("UHF-T (450-470 MHz)");
            break;
        case BAND_UHF_PUBLIC:
            scan_start_freq = 470000000;
            scan_end_freq = 512000000;
            scan_step = 12500;
            text_band.set("UHF Public (470-512 MHz)");
            break;
        case BAND_700_MHZ:
            scan_start_freq = 700000000;
            scan_end_freq = 800000000;
            scan_step = 25000;
            text_band.set("700 MHz Public Safety");
            break;
        case BAND_800_MHZ:
            scan_start_freq = 806000000;
            scan_end_freq = 869000000;
            scan_step = 12500;
            text_band.set("800 MHz Trunked");
            break;
        case BAND_900_MHZ:
            scan_start_freq = 896000000;
            scan_end_freq = 960000000;
            scan_step = 25000;
            text_band.set("900 MHz");
            break;
        case BAND_FULL:
        default:
            scan_start_freq = 150000000;
            scan_end_freq = 900000000;
            scan_step = 25000;
            text_band.set("Full Scan");
            break;
    }
    current_freq = scan_start_freq;
}

void RFScannerView::start_scan() {
    scanning = true;
    scan_count = 0;
    current_freq = scan_start_freq;

    button_scan.set_text("STOP");
    console.writeln("Scanning started...");

    // Configure receiver
    receiver_model.set_sampling_rate(3072000);
    receiver_model.set_baseband_bandwidth(1750000);
    receiver_model.set_modulation(ReceiverModel::Mode::WidebandFMAudio);
    receiver_model.set_squelch_level(0);
    receiver_model.enable();
}

void RFScannerView::stop_scan() {
    scanning = false;
    button_scan.set_text("START");
    console.writeln("Scanning stopped");

    receiver_model.disable();
}

void RFScannerView::on_tick() {
    if (!scanning) return;

    // Get current RSSI
    auto rssi_raw = rssi.get_value();
    int16_t current_rssi = rssi_raw;

    // Process signal if above threshold
    if (current_rssi > rssi_threshold) {
        process_signal(current_freq, current_rssi);
    }

    // Move to next frequency
    current_freq += scan_step;
    if (current_freq > scan_end_freq) {
        current_freq = scan_start_freq;
        scan_count++;
    }

    // Tune to next frequency
    receiver_model.set_target_frequency(current_freq);

    // Update UI
    update_ui();
}

void RFScannerView::process_signal(uint32_t freq, int16_t rssi) {
    // Update signal history
    update_signal_history(freq, rssi);

    // Identify signal using frequency database
    identify_signal(freq, rssi);
}

void RFScannerView::identify_signal(uint32_t freq, int16_t rssi) {
    // Check against frequency database
    const FreqEntry* entry = find_frequency(freq);

    if (entry) {
        // Calculate confidence based on RSSI and frequency match accuracy
        uint8_t confidence = 50;

        // Higher RSSI = higher confidence
        if (rssi > -60) confidence += 30;
        else if (rssi > -70) confidence += 20;
        else if (rssi > -80) confidence += 10;

        // Exact frequency match = higher confidence
        if (freq == entry->frequency) confidence += 20;

        if (confidence > 100) confidence = 100;

        // Check if we need minimum detections
        SignalHistory* hist = nullptr;
        for (size_t i = 0; i < history_count; i++) {
            if (signal_history[i].frequency == freq) {
                hist = &signal_history[i];
                break;
            }
        }

        if (hist && hist->detection_count >= min_detections) {
            add_detection(freq, entry->device_type, entry->description, confidence);
        }
    }
}

void RFScannerView::add_detection(uint32_t freq, const char* device, const char* desc, uint8_t confidence) {
    // Check if already detected
    for (size_t i = 0; i < detected_count; i++) {
        if (detected_signals[i].frequency == freq && detected_signals[i].is_active) {
            // Update existing detection
            detected_signals[i].rssi = rssi.get_value();
            detected_signals[i].timestamp = chTimeNow();
            detected_signals[i].confidence = confidence;
            return;
        }
    }

    // Add new detection if space available
    if (detected_count < MAX_DETECTED) {
        detected_signals[detected_count].frequency = freq;
        detected_signals[detected_count].rssi = rssi.get_value();
        detected_signals[detected_count].timestamp = chTimeNow();
        detected_signals[detected_count].device_type = device;
        detected_signals[detected_count].description = desc;
        detected_signals[detected_count].confidence = confidence;
        detected_signals[detected_count].is_active = true;

        detected_count++;
        detections_count++;

        // Update UI
        text_last_device.set(device);
        text_description.set(desc);
        text_detections.set(to_string_dec_uint(detections_count));

        // Log to console
        char buffer[80];
        snprintf(buffer, sizeof(buffer), "%s @ %.4f MHz",
                 device, freq / 1000000.0);
        console.writeln(buffer);
    }
}

void RFScannerView::update_signal_history(uint32_t freq, int16_t rssi) {
    // Find existing history entry
    SignalHistory* hist = nullptr;
    for (size_t i = 0; i < history_count; i++) {
        if (signal_history[i].frequency == freq) {
            hist = &signal_history[i];
            break;
        }
    }

    // Create new history entry if needed
    if (!hist && history_count < MAX_HISTORY) {
        hist = &signal_history[history_count];
        hist->frequency = freq;
        hist->history_index = 0;
        hist->detection_count = 0;
        history_count++;
    }

    // Update history
    if (hist) {
        hist->rssi_history[hist->history_index] = rssi;
        hist->history_index = (hist->history_index + 1) % 10;
        hist->last_seen = chTimeNow();
        hist->detection_count++;
    }
}

void RFScannerView::update_ui() {
    // Update frequency display
    auto freq_str = to_string_dec_uint(current_freq / 1000000) + "." +
                    to_string_dec_uint((current_freq % 1000000) / 1000, 3, '0') + " MHz";
    text_frequency.set(freq_str);

    // Update RSSI display
    auto rssi_val = rssi.get_value();
    text_rssi.set(to_string_dec_int(rssi_val) + " dBm");
}

void RFScannerView::save_detections() {
    if (detected_count == 0) {
        console.writeln("No detections to save");
        return;
    }

    // Create filename with timestamp
    rtc::RTC datetime;
    rtcGetTime(&RTCD1, &datetime);

    auto filename = "RFSCN_" + to_string_dec_uint(datetime.year(), 4) +
                    to_string_dec_uint(datetime.month(), 2, '0') +
                    to_string_dec_uint(datetime.day(), 2, '0') + "_" +
                    to_string_dec_uint(datetime.hour(), 2, '0') +
                    to_string_dec_uint(datetime.minute(), 2, '0') + ".TXT";

    auto path = u"RFSCANS/" + filename;

    // Create directory if needed
    make_new_directory(u"RFSCANS");

    // Write detections
    File file;
    auto result = file.create(path);
    if (!result.is_valid()) {
        console.writeln("Failed to create file");
        return;
    }

    file.write_line("RF Scanner Detections");
    file.write_line("====================");
    file.write_line("");

    for (size_t i = 0; i < detected_count; i++) {
        if (detected_signals[i].is_active) {
            char buffer[200];
            snprintf(buffer, sizeof(buffer),
                     "Freq: %.4f MHz | RSSI: %d dBm | Confidence: %d%%\nDevice: %s\nDescription: %s\n",
                     detected_signals[i].frequency / 1000000.0,
                     detected_signals[i].rssi,
                     detected_signals[i].confidence,
                     detected_signals[i].device_type,
                     detected_signals[i].description);
            file.write_line(buffer);
        }
    }

    console.writeln("Saved: " + filename);
}

} /* namespace ui */
