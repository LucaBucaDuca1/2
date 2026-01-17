/*
 * ESP32 Wireless Device Scanner with LIVE DISTANCE TRACKING
 * Advanced version with real-time distance analysis and trend detection
 *
 * Features:
 * - WiFi and BLE scanning
 * - Device-specific distance calibration
 * - Real-time distance updates (approaching/receding detection)
 * - Distance history and trend analysis
 * - Proximity alerts
 * - Live distance monitoring
 */

#include <WiFi.h>
#include <esp_wifi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "device_patterns.h"
#include "distance_analysis.h"

// Pin Configuration
#define ALERT_LED_PIN 2
#define BUZZER_PIN 4
#define STATUS_LED_PIN 15

// Scan Configuration
#define WIFI_SCAN_INTERVAL 3000    // Faster scanning for live updates
#define BLE_SCAN_INTERVAL 3000
#define BLE_SCAN_DURATION 2
#define RSSI_THRESHOLD -90
#define SERIAL_BAUD 115200

// Distance Alert Configuration
#define PROXIMITY_ALERT_DISTANCE 10.0    // Alert when device within 10m
#define CLOSE_ALERT_DISTANCE 5.0         // Close alert at 5m
#define IMMEDIATE_ALERT_DISTANCE 2.0     // Immediate alert at 2m

// Alert Configuration
#define ALERT_ON_EMERGENCY_VEHICLE true
#define ALERT_ON_CAMERA true
#define ALERT_ON_BODYCAM true
#define ALERT_ON_TRAFFIC_MONITOR true
#define ALERT_ON_CLOSE_PROXIMITY true    // Alert when ANY device is very close

// Device tracking
struct DeviceInfo {
  String address;
  String name;
  String vendor;
  int8_t rssi;
  String type;
  unsigned long lastSeen;
  unsigned long firstSeen;
  bool isNew;
  int detectionCount;
  float lastDistance;
  String lastTrend;
};

std::map<String, DeviceInfo> knownDevices;
BLEScan* pBLEScan;

// Alert state
bool alertActive = false;
unsigned long alertStartTime = 0;
unsigned long lastDistanceUpdate = 0;

// Device type identification
String identifyDeviceType(String name, String address) {
  String nameLower = name;
  nameLower.toLowerCase();

  String macType = getTypeFromMAC(address);
  if (macType != "UNKNOWN") return macType;

  for (int i = 0; i < namePatternsSize; i++) {
    String keyword = String(namePatterns[i].keyword);
    if (!namePatterns[i].caseSensitive) {
      keyword.toLowerCase();
    }
    if (nameLower.indexOf(keyword) >= 0) {
      return String(namePatterns[i].deviceType);
    }
  }

  return "UNKNOWN";
}

// Alert control
void triggerAlert(String deviceType, String name, float distance) {
  alertActive = true;
  alertStartTime = millis();

  digitalWrite(ALERT_LED_PIN, HIGH);

  if (BUZZER_PIN >= 0) {
    // Different patterns based on distance
    if (distance < IMMEDIATE_ALERT_DISTANCE) {
      // Very urgent: rapid beeps
      for (int i = 0; i < 5; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(50);
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);
      }
    } else if (distance < CLOSE_ALERT_DISTANCE) {
      // Urgent: 3 beeps
      for (int i = 0; i < 3; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
      }
    } else if (deviceType == "EMERGENCY_VEHICLE" || deviceType == "BODYCAM") {
      // Important device: 2 beeps
      for (int i = 0; i < 2; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(150);
        digitalWrite(BUZZER_PIN, LOW);
        delay(150);
      }
    } else {
      // Standard: 1 beep
      digitalWrite(BUZZER_PIN, HIGH);
      delay(300);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  Serial.println("!!! ALERT: " + deviceType + " @ " + formatDistance(distance) + " !!!");
}

void clearAlert() {
  if (alertActive && (millis() - alertStartTime > 3000)) {
    alertActive = false;
    digitalWrite(ALERT_LED_PIN, LOW);
  }
}

// Check if device should trigger alert
bool shouldAlert(String deviceType, float distance) {
  // Proximity-based alerts
  if (ALERT_ON_CLOSE_PROXIMITY && distance < CLOSE_ALERT_DISTANCE) return true;

  // Type-based alerts
  if (deviceType == "EMERGENCY_VEHICLE" && ALERT_ON_EMERGENCY_VEHICLE) return true;
  if (deviceType == "CAMERA" && ALERT_ON_CAMERA && distance < PROXIMITY_ALERT_DISTANCE) return true;
  if (deviceType == "BODYCAM" && ALERT_ON_BODYCAM) return true;
  if (deviceType == "TRAFFIC_MONITOR" && ALERT_ON_TRAFFIC_MONITOR && distance < 50) return true;

  return false;
}

// Print detailed device info with live distance
void printDeviceInfo(DeviceInfo info, String source, bool showFull) {
  // Update distance tracking
  updateDistanceTracking(info.address, info.rssi, info.type);
  DistanceData distData = getDistanceInfo(info.address);

  // Store in device info
  info.lastDistance = distData.smoothedDistance;
  info.lastTrend = distData.trend;

  if (!showFull && info.type == "UNKNOWN" && distData.smoothedDistance > 50) {
    return; // Skip unknown distant devices
  }

  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║ " + source + " DEVICE DETECTED");
  Serial.println("╠════════════════════════════════════════════════════════════╣");

  Serial.println("║ Name: " + info.name);
  Serial.println("║ Address: " + info.address);
  Serial.println("║ Vendor: " + info.vendor);
  Serial.println("║ Type: " + info.type);
  Serial.println("╠════════════════════════════════════════════════════════════╣");

  // Signal strength
  Serial.println("║ RSSI: " + String(info.rssi) + " dBm");
  Serial.println("║ Signal Quality: " + getSignalQuality(info.rssi, distData.smoothedDistance));

  // Distance information
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 📏 DISTANCE ANALYSIS:");
  Serial.println("║   Current: " + formatDistance(distData.currentDistance) +
                 " " + getDistanceIndicator(distData.currentDistance));
  Serial.println("║   Smoothed: " + formatDistance(distData.smoothedDistance) +
                 " [" + getDistanceCategory(distData.smoothedDistance) + "]");

  if (distData.updateCount > 1) {
    Serial.println("║   Closest: " + formatDistance(distData.minDistance));
    Serial.println("║   Farthest: " + formatDistance(distData.maxDistance));
  }

  // Movement tracking
  if (distData.updateCount > 2) {
    Serial.println("╠════════════════════════════════════════════════════════════╣");
    Serial.println("║ 🎯 MOVEMENT TRACKING:");
    Serial.println("║   Trend: " + getTrendIndicator(distData.trend));
    Serial.println("║   Velocity: " + String(abs(distData.velocity), 2) + " m/s (" +
                   getVelocityDescription(distData.velocity) + ")");

    // Prediction
    if (distData.trend == "APPROACHING") {
      float predicted5s = predictDistance(info.address, 5);
      Serial.println("║   Est. in 5s: " + formatDistance(predicted5s));

      if (predicted5s < CLOSE_ALERT_DISTANCE && predicted5s > 0) {
        Serial.println("║   ⚠️  WARNING: Will be CLOSE in 5 seconds!");
      }
    }
  }

  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ Status: " + String(info.isNew ? "NEW DEVICE" : "KNOWN DEVICE"));
  Serial.println("║ Detections: " + String(info.detectionCount) +
                 " | Updates: " + String(distData.updateCount));

  unsigned long age = (millis() - info.firstSeen) / 1000;
  Serial.println("║ First seen: " + String(age) + "s ago");

  Serial.println("╚════════════════════════════════════════════════════════════╝");

  // Alert check
  if (shouldAlert(info.type, distData.smoothedDistance)) {
    triggerAlert(info.type, info.name, distData.smoothedDistance);
  }
}

// Print compact live distance update
void printLiveDistanceUpdate(String address) {
  if (knownDevices.find(address) == knownDevices.end()) return;

  DeviceInfo info = knownDevices[address];
  DistanceData distData = getDistanceInfo(address);

  String indicator = getDistanceIndicator(distData.smoothedDistance);
  String trendArrow = "";

  if (distData.trend == "APPROACHING") trendArrow = "⬇️";
  else if (distData.trend == "RECEDING") trendArrow = "⬆️";
  else trendArrow = "↔️";

  Serial.println("[LIVE] " + indicator + " " + trendArrow + " " +
                 info.name.substring(0, 20) + " @ " +
                 formatDistance(distData.smoothedDistance) +
                 " (" + String(distData.velocity, 1) + " m/s)");
}

// BLE callback with distance tracking
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String address = advertisedDevice.getAddress().toString().c_str();
      String name = advertisedDevice.haveName() ? advertisedDevice.getName().c_str() : "Unnamed Device";
      int8_t rssi = advertisedDevice.getRSSI();

      if (rssi < RSSI_THRESHOLD) return;

      bool isNew = (knownDevices.find(address) == knownDevices.end());

      DeviceInfo info;
      if (isNew) {
        info.address = address;
        info.name = name;
        info.vendor = lookupVendor(address);
        info.type = identifyDeviceType(name, address);
        info.firstSeen = millis();
        info.detectionCount = 1;
        info.isNew = true;
      } else {
        info = knownDevices[address];
        info.detectionCount++;
        info.isNew = false;
      }

      info.rssi = rssi;
      info.lastSeen = millis();

      knownDevices[address] = info;

      // Print full info for new or important devices
      if (isNew || info.type != "UNKNOWN" || shouldAlert(info.type, 999)) {
        printDeviceInfo(info, "BLE", isNew);
      } else {
        // Just update distance tracking silently
        updateDistanceTracking(address, rssi, info.type);
      }
    }
};

void scanWiFi() {
  digitalWrite(STATUS_LED_PIN, HIGH);
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║   WiFi Scan...                 ║");
  Serial.println("╚════════════════════════════════╝");

  int n = WiFi.scanNetworks(false, true, false, 300);

  for (int i = 0; i < n; i++) {
    String ssid = WiFi.SSID(i);
    String bssid = WiFi.BSSIDstr(i);
    int8_t rssi = WiFi.RSSI(i);

    if (rssi < RSSI_THRESHOLD) continue;

    bool isNew = (knownDevices.find(bssid) == knownDevices.end());

    DeviceInfo info;
    if (isNew) {
      info.address = bssid;
      info.name = ssid;
      info.vendor = lookupVendor(bssid);
      info.type = identifyDeviceType(ssid, bssid);
      info.firstSeen = millis();
      info.detectionCount = 1;
      info.isNew = true;
    } else {
      info = knownDevices[bssid];
      info.detectionCount++;
      info.isNew = false;
    }

    info.rssi = rssi;
    info.lastSeen = millis();

    knownDevices[bssid] = info;

    if (isNew || info.type != "UNKNOWN" || shouldAlert(info.type, 999)) {
      printDeviceInfo(info, "WiFi", isNew);
    } else {
      updateDistanceTracking(bssid, rssi, info.type);
    }
  }

  WiFi.scanDelete();
  digitalWrite(STATUS_LED_PIN, LOW);
}

void scanBLE() {
  digitalWrite(STATUS_LED_PIN, HIGH);
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║   BLE Scan...                  ║");
  Serial.println("╚════════════════════════════════╝");

  BLEScanResults foundDevices = pBLEScan->start(BLE_SCAN_DURATION, false);
  pBLEScan->clearResults();
  digitalWrite(STATUS_LED_PIN, LOW);
}

void printLiveDistances() {
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║ 🎯 LIVE DISTANCE MONITORING");
  Serial.println("╚════════════════════════════════════════════════════════════╝");

  // Show all tracked devices sorted by distance
  std::vector<std::pair<String, float>> deviceDistances;

  for (auto const& pair : knownDevices) {
    DistanceData distData = getDistanceInfo(pair.first);
    if (distData.updateCount > 0) {
      deviceDistances.push_back({pair.first, distData.smoothedDistance});
    }
  }

  // Sort by distance (closest first)
  std::sort(deviceDistances.begin(), deviceDistances.end(),
    [](const auto& a, const auto& b) { return a.second < b.second; });

  // Display top 10 closest devices
  int count = 0;
  for (auto const& pair : deviceDistances) {
    if (count++ >= 10) break;
    printLiveDistanceUpdate(pair.first);
  }

  if (deviceDistances.empty()) {
    Serial.println("No devices currently tracked");
  }
}

void printSummary() {
  Serial.println("\n");
  Serial.println("╔════════════════════════════════════════════════════════════╗");
  Serial.println("║          📊 COMPREHENSIVE SUMMARY REPORT                   ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝");

  Serial.println("║ Total tracked devices: " + String(knownDevices.size()));

  // Get distance statistics
  DistanceStats stats = getDistanceStatistics();

  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 📏 DISTANCE DISTRIBUTION:");
  Serial.println("║   🔴 Immediate (< 2m):    " + String(stats.immediate));
  Serial.println("║   🟠 Very Close (2-10m):  " + String(stats.veryClose));
  Serial.println("║   🟡 Close (10-25m):      " + String(stats.close));
  Serial.println("║   🟢 Nearby (25-50m):     " + String(stats.nearby));
  Serial.println("║   🔵 Moderate (50-100m):  " + String(stats.moderate));
  Serial.println("║   ⚪ Far (> 100m):        " + String(stats.far));

  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 🎯 MOVEMENT ANALYSIS:");
  Serial.println("║   ⬇️  Approaching:         " + String(stats.approaching));
  Serial.println("║   ⬆️  Receding:            " + String(stats.receding));
  Serial.println("║   ↔️  Stable:              " +
                 String(stats.totalDevices - stats.approaching - stats.receding));

  if (stats.closestDistance < 999999) {
    Serial.println("╠════════════════════════════════════════════════════════════╣");
    Serial.println("║ ⭐ CLOSEST DEVICE:");
    Serial.println("║   Distance: " + formatDistance(stats.closestDistance));
    Serial.println("║   Address: " + stats.closestDevice);
  }

  // Device type breakdown
  std::map<String, int> typeCounts;
  for (auto const& pair : knownDevices) {
    typeCounts[pair.second.type]++;
  }

  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 📱 DEVICE TYPES:");
  for (auto const& pair : typeCounts) {
    if (pair.second > 0) {
      String indicator = shouldAlert(pair.first, 999) ? " ⚠️ " : "   ";
      Serial.println("║ " + indicator + pair.first + ": " + String(pair.second));
    }
  }

  Serial.println("╚════════════════════════════════════════════════════════════╝\n");
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  pinMode(ALERT_LED_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  if (BUZZER_PIN >= 0) {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
  }
  digitalWrite(ALERT_LED_PIN, LOW);
  digitalWrite(STATUS_LED_PIN, LOW);

  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════════════════════════╗");
  Serial.println("║   ESP32 LIVE DISTANCE TRACKER v3.0                        ║");
  Serial.println("║   Real-Time Distance Analysis & Movement Detection        ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝");
  Serial.println("\nInitializing...");

  digitalWrite(ALERT_LED_PIN, HIGH);
  delay(200);
  digitalWrite(ALERT_LED_PIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("✓ WiFi initialized");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  Serial.println("✓ BLE initialized");

  Serial.println("\n🔍 Live Distance Tracking Active\n");
  Serial.println("Distance Alerts:");
  Serial.println("  🔴 < 2m   = IMMEDIATE");
  Serial.println("  🟠 < 5m   = CLOSE");
  Serial.println("  🟡 < 10m  = PROXIMITY");
  if (ALERT_ON_EMERGENCY_VEHICLE) Serial.println("  ⚠️  Emergency Vehicles");
  if (ALERT_ON_CAMERA) Serial.println("  ⚠️  Cameras (< 10m)");
  if (ALERT_ON_BODYCAM) Serial.println("  ⚠️  Body Cameras");
  Serial.println("");
}

unsigned long lastWiFiScan = 0;
unsigned long lastBLEScan = 0;
unsigned long lastSummary = 0;
unsigned long lastLiveUpdate = 0;

void loop() {
  unsigned long now = millis();

  clearAlert();

  // WiFi scan
  if (now - lastWiFiScan >= WIFI_SCAN_INTERVAL) {
    scanWiFi();
    lastWiFiScan = now;
  }

  // BLE scan
  if (now - lastBLEScan >= BLE_SCAN_INTERVAL) {
    scanBLE();
    lastBLEScan = now;
  }

  // Live distance updates every 5 seconds
  if (now - lastLiveUpdate >= 5000) {
    printLiveDistances();
    lastLiveUpdate = now;
  }

  // Summary every 30 seconds
  if (now - lastSummary >= 30000) {
    printSummary();
    lastSummary = now;

    // Cleanup old devices (not seen in 60 seconds)
    cleanupOldDistanceData(60000);
  }

  delay(100);
}
