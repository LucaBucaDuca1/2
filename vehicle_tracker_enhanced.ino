/*
 * ESP32 Wireless Device Scanner - Enhanced Version
 * Scans for WiFi and Bluetooth devices with visual/audio alerts
 *
 * Features:
 * - WiFi AP and Station scanning
 * - Bluetooth Classic and BLE scanning
 * - Device type identification with MAC vendor lookup
 * - Signal strength monitoring
 * - Visual alerts (LED)
 * - Optional audio alerts (buzzer)
 * - Distance estimation
 * - Device tracking and history
 */

#include <WiFi.h>
#include <esp_wifi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "device_patterns.h"

// Pin Configuration
#define ALERT_LED_PIN 2        // Built-in LED on most ESP32 boards
#define BUZZER_PIN 4           // Optional buzzer (set to -1 to disable)
#define STATUS_LED_PIN 15      // Status LED (scanning indicator)

// Configuration
#define WIFI_SCAN_INTERVAL 5000
#define BLE_SCAN_INTERVAL 5000
#define BLE_SCAN_DURATION 3
#define RSSI_THRESHOLD -90
#define SERIAL_BAUD 115200

// Alert Configuration
#define ALERT_ON_EMERGENCY_VEHICLE true
#define ALERT_ON_CAMERA true
#define ALERT_ON_BODYCAM true
#define ALERT_ON_TRAFFIC_MONITOR true

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
};

std::map<String, DeviceInfo> knownDevices;
BLEScan* pBLEScan;

// Alert state
bool alertActive = false;
unsigned long alertStartTime = 0;

// Device type identification
String identifyDeviceType(String name, String address) {
  String nameLower = name;
  nameLower.toLowerCase();

  // First check MAC address vendor database
  String macType = getTypeFromMAC(address);
  if (macType != "UNKNOWN") {
    return macType;
  }

  // Then check name patterns
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

// Signal strength description
String getSignalStrength(int8_t rssi) {
  if (rssi >= -50) return "VERY_STRONG";
  if (rssi >= -60) return "STRONG";
  if (rssi >= -70) return "MODERATE";
  if (rssi >= -80) return "WEAK";
  return "VERY_WEAK";
}

// Distance estimation (approximate)
float estimateDistance(int8_t rssi) {
  int txPower = -59;
  if (rssi == 0) return -1.0;

  float ratio = rssi * 1.0 / txPower;
  if (ratio < 1.0) {
    return pow(ratio, 10);
  } else {
    return (0.89976) * pow(ratio, 7.7095) + 0.111;
  }
}

// Alert control
void triggerAlert(String deviceType, String name) {
  alertActive = true;
  alertStartTime = millis();

  digitalWrite(ALERT_LED_PIN, HIGH);

  if (BUZZER_PIN >= 0) {
    // Beep pattern based on device type
    if (deviceType == "EMERGENCY_VEHICLE" || deviceType == "BODYCAM") {
      // Urgent pattern: 3 quick beeps
      for (int i = 0; i < 3; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
      }
    } else {
      // Standard pattern: 1 long beep
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  Serial.println("!!! ALERT TRIGGERED: " + deviceType + " - " + name + " !!!");
}

void clearAlert() {
  if (alertActive && (millis() - alertStartTime > 3000)) {
    alertActive = false;
    digitalWrite(ALERT_LED_PIN, LOW);
  }
}

// Check if device should trigger alert
bool shouldAlert(String deviceType) {
  if (deviceType == "EMERGENCY_VEHICLE" && ALERT_ON_EMERGENCY_VEHICLE) return true;
  if (deviceType == "CAMERA" && ALERT_ON_CAMERA) return true;
  if (deviceType == "BODYCAM" && ALERT_ON_BODYCAM) return true;
  if (deviceType == "TRAFFIC_MONITOR" && ALERT_ON_TRAFFIC_MONITOR) return true;
  return false;
}

// Print device info
void printDeviceInfo(DeviceInfo info, String source) {
  Serial.println("\n[" + source + " DEVICE DETECTED]");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Name: " + info.name);
  Serial.println("Address: " + info.address);
  Serial.println("Vendor: " + info.vendor);
  Serial.println("Type: " + info.type);
  Serial.println("RSSI: " + String(info.rssi) + " dBm (" + getSignalStrength(info.rssi) + ")");

  float distance = estimateDistance(info.rssi);
  if (distance >= 0) {
    Serial.println("Est. Distance: ~" + String(distance, 1) + " meters");
  }

  Serial.println("Status: " + String(info.isNew ? "NEW DEVICE" : "KNOWN DEVICE"));
  Serial.println("Detections: " + String(info.detectionCount));

  unsigned long age = (millis() - info.firstSeen) / 1000;
  Serial.println("First seen: " + String(age) + " seconds ago");

  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

  if (shouldAlert(info.type)) {
    triggerAlert(info.type, info.name);
  }
}

// BLE callback
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

      // Print for new devices or important types
      if (isNew || info.type != "UNKNOWN" || shouldAlert(info.type)) {
        printDeviceInfo(info, "BLE");
      }
    }
};

void scanWiFi() {
  digitalWrite(STATUS_LED_PIN, HIGH);
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║   Starting WiFi Scan...        ║");
  Serial.println("╚════════════════════════════════╝");

  int n = WiFi.scanNetworks(false, true, false, 300);
  Serial.println("Networks found: " + String(n));

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

    if (isNew || info.type != "UNKNOWN" || shouldAlert(info.type)) {
      printDeviceInfo(info, "WiFi");
    }
  }

  WiFi.scanDelete();
  digitalWrite(STATUS_LED_PIN, LOW);
}

void scanBLE() {
  digitalWrite(STATUS_LED_PIN, HIGH);
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║   Starting BLE Scan...         ║");
  Serial.println("╚════════════════════════════════╝");

  BLEScanResults foundDevices = pBLEScan->start(BLE_SCAN_DURATION, false);
  Serial.println("BLE scan complete. Devices: " + String(foundDevices.getCount()));

  pBLEScan->clearResults();
  digitalWrite(STATUS_LED_PIN, LOW);
}

void printSummary() {
  Serial.println("\n");
  Serial.println("╔════════════════════════════════════════════╗");
  Serial.println("║          DEVICE SUMMARY REPORT             ║");
  Serial.println("╚════════════════════════════════════════════╝");

  Serial.println("Total tracked devices: " + String(knownDevices.size()));

  std::map<String, int> typeCounts;
  int activeDevices = 0;
  unsigned long now = millis();

  for (auto const& pair : knownDevices) {
    DeviceInfo info = pair.second;
    typeCounts[info.type]++;
    if (now - info.lastSeen < 30000) {
      activeDevices++;
    }
  }

  Serial.println("Active devices (last 30s): " + String(activeDevices));
  Serial.println("\n--- Device Types ---");

  for (auto const& pair : typeCounts) {
    if (pair.second > 0) {
      String indicator = shouldAlert(pair.first) ? " ⚠️ " : "   ";
      Serial.println(indicator + pair.first + ": " + String(pair.second));
    }
  }

  Serial.println("════════════════════════════════════════════\n");
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  // Setup pins
  pinMode(ALERT_LED_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  if (BUZZER_PIN >= 0) {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
  }
  digitalWrite(ALERT_LED_PIN, LOW);
  digitalWrite(STATUS_LED_PIN, LOW);

  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════════╗");
  Serial.println("║   ESP32 Wireless Device Scanner v2.0      ║");
  Serial.println("║   WiFi + BLE Scanner with Alerts          ║");
  Serial.println("╚════════════════════════════════════════════╝");
  Serial.println("\nInitializing...");

  // Test alert
  Serial.println("Testing alert system...");
  digitalWrite(ALERT_LED_PIN, HIGH);
  delay(200);
  digitalWrite(ALERT_LED_PIN, LOW);

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("✓ WiFi initialized");

  // Initialize BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  Serial.println("✓ BLE initialized");

  Serial.println("\n🔍 Scanning for devices...\n");
  Serial.println("Alert Types:");
  if (ALERT_ON_EMERGENCY_VEHICLE) Serial.println("  ⚠️  Emergency Vehicles");
  if (ALERT_ON_CAMERA) Serial.println("  ⚠️  Cameras");
  if (ALERT_ON_BODYCAM) Serial.println("  ⚠️  Body Cameras");
  if (ALERT_ON_TRAFFIC_MONITOR) Serial.println("  ⚠️  Traffic Monitors");
  Serial.println("");
}

unsigned long lastWiFiScan = 0;
unsigned long lastBLEScan = 0;
unsigned long lastSummary = 0;

void loop() {
  unsigned long now = millis();

  // Clear any active alerts
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

  // Summary every 30 seconds
  if (now - lastSummary >= 30000) {
    printSummary();
    lastSummary = now;
  }

  delay(100);
}
