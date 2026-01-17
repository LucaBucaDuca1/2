/*
 * Configuration file for ESP32 Wireless Device Scanner
 * Modify these settings according to your needs
 */

#ifndef CONFIG_H
#define CONFIG_H

// Serial Configuration
#define SERIAL_BAUD 115200

// Scan Intervals (milliseconds)
#define WIFI_SCAN_INTERVAL 5000    // WiFi scan every 5 seconds
#define BLE_SCAN_INTERVAL 5000     // BLE scan every 5 seconds
#define BLE_SCAN_DURATION 3        // BLE scan duration in seconds

// Signal Filtering
#define RSSI_THRESHOLD -90         // Minimum signal strength to report (-90 to -30 dBm)
                                   // -90: Very weak, long range
                                   // -70: Moderate
                                   // -50: Strong, close range

// Alert Configuration
#define ENABLE_ALERTS true         // Enable audio/visual alerts
#define ALERT_LED_PIN 2            // GPIO pin for alert LED (built-in LED on most ESP32)

// Device Type Alerts
// Set to true to enable alerts for specific device types
#define ALERT_ON_EMERGENCY_VEHICLE true
#define ALERT_ON_CAMERA true
#define ALERT_ON_BODYCAM true
#define ALERT_ON_TRAFFIC_MONITOR true
#define ALERT_ON_MOBILE_HOTSPOT false
#define ALERT_ON_VEHICLE false

// Display Options
#define SHOW_ALL_DEVICES false     // Show all devices or only interesting ones
#define SHOW_SUMMARY true          // Show periodic summary
#define SUMMARY_INTERVAL 30000     // Summary interval in milliseconds

// Advanced Options
#define ACTIVE_DEVICE_TIMEOUT 30000  // Consider device active if seen within this time (ms)
#define ENABLE_BLE_SCAN true         // Enable BLE scanning
#define ENABLE_WIFI_SCAN true        // Enable WiFi scanning

// Data Logging
#define ENABLE_SD_LOGGING false      // Enable logging to SD card (requires SD card module)
#define LOG_FILE_NAME "/scanner_log.txt"

// Display Configuration (optional - for OLED/LCD displays)
#define ENABLE_DISPLAY false         // Enable external display
#define DISPLAY_TYPE "OLED"          // "OLED" or "LCD"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#endif // CONFIG_H
