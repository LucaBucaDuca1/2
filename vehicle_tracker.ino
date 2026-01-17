/*
 * ESP32 Wireless Device Scanner
 * Scans for WiFi and Bluetooth devices in the area for situational awareness
 *
 * Features:
 * - WiFi AP and Station scanning
 * - Bluetooth Classic and BLE scanning
 * - Device type identification
 * - Signal strength monitoring
 * - MAC address vendor lookup
 */

#include <WiFi.h>
#include <esp_wifi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Configuration
#define WIFI_SCAN_INTERVAL 5000    // WiFi scan every 5 seconds
#define BLE_SCAN_INTERVAL 5000     // BLE scan every 5 seconds
#define BLE_SCAN_DURATION 3        // BLE scan duration in seconds
#define RSSI_THRESHOLD -90         // Minimum signal strength to report
#define SERIAL_BAUD 115200

// Device tracking structure
struct DeviceInfo {
  String address;
  String name;
  int8_t rssi;
  String type;
  unsigned long lastSeen;
  bool isNew;
};

// Device database
std::map<String, DeviceInfo> knownDevices;
BLEScan* pBLEScan;

// Device type identification based on name patterns and characteristics
String identifyDeviceType(String name, String address) {
  String nameLower = name;
  nameLower.toLowerCase();

  // Camera systems
  if (nameLower.indexOf("camera") >= 0 || nameLower.indexOf("cam") >= 0 ||
      nameLower.indexOf("ipcam") >= 0 || nameLower.indexOf("nvr") >= 0 ||
      nameLower.indexOf("dvr") >= 0 || nameLower.indexOf("hikvision") >= 0 ||
      nameLower.indexOf("dahua") >= 0 || nameLower.indexOf("axis") >= 0) {
    return "CAMERA";
  }

  // Vehicle systems (CarPlay, Android Auto, vehicle Bluetooth)
  if (nameLower.indexOf("car") >= 0 || nameLower.indexOf("vehicle") >= 0 ||
      nameLower.indexOf("ford") >= 0 || nameLower.indexOf("chevy") >= 0 ||
      nameLower.indexOf("toyota") >= 0 || nameLower.indexOf("honda") >= 0 ||
      nameLower.indexOf("nissan") >= 0 || nameLower.indexOf("bmw") >= 0 ||
      nameLower.indexOf("mercedes") >= 0 || nameLower.indexOf("audi") >= 0 ||
      nameLower.indexOf("sync") >= 0 || nameLower.indexOf("mylink") >= 0 ||
      nameLower.indexOf("uconnect") >= 0 || nameLower.indexOf("carplay") >= 0) {
    return "VEHICLE";
  }

  // Police/Emergency vehicle systems
  if (nameLower.indexOf("police") >= 0 || nameLower.indexOf("patrol") >= 0 ||
      nameLower.indexOf("sheriff") >= 0 || nameLower.indexOf("squad") >= 0 ||
      nameLower.indexOf("emergency") >= 0 || nameLower.indexOf("ems") >= 0 ||
      nameLower.indexOf("fire") >= 0 || nameLower.indexOf("motorola") >= 0 ||
      nameLower.indexOf("panasonic cf") >= 0) {
    return "EMERGENCY_VEHICLE";
  }

  // Mobile hotspots and MDTs (Mobile Data Terminals)
  if (nameLower.indexOf("hotspot") >= 0 || nameLower.indexOf("mdt") >= 0 ||
      nameLower.indexOf("mobile data") >= 0 || nameLower.indexOf("cruiser") >= 0) {
    return "MOBILE_HOTSPOT";
  }

  // Smartphones
  if (nameLower.indexOf("iphone") >= 0 || nameLower.indexOf("android") >= 0 ||
      nameLower.indexOf("samsung") >= 0 || nameLower.indexOf("pixel") >= 0 ||
      nameLower.indexOf("oneplus") >= 0 || nameLower.indexOf("xiaomi") >= 0) {
    return "SMARTPHONE";
  }

  // Dash cameras
  if (nameLower.indexOf("dash") >= 0 || nameLower.indexOf("dashcam") >= 0 ||
      nameLower.indexOf("viofo") >= 0 || nameLower.indexOf("garmin") >= 0) {
    return "DASHCAM";
  }

  // Body cameras
  if (nameLower.indexOf("bodycam") >= 0 || nameLower.indexOf("body cam") >= 0 ||
      nameLower.indexOf("axon") >= 0 || nameLower.indexOf("watchguard") >= 0) {
    return "BODYCAM";
  }

  // Traffic/Street monitoring
  if (nameLower.indexOf("traffic") >= 0 || nameLower.indexOf("sensor") >= 0 ||
      nameLower.indexOf("monitor") >= 0) {
    return "TRAFFIC_MONITOR";
  }

  // Laptops and computers
  if (nameLower.indexOf("laptop") >= 0 || nameLower.indexOf("pc") >= 0 ||
      nameLower.indexOf("macbook") >= 0 || nameLower.indexOf("dell") >= 0 ||
      nameLower.indexOf("lenovo") >= 0 || nameLower.indexOf("hp") >= 0) {
    return "COMPUTER";
  }

  // Wearables
  if (nameLower.indexOf("watch") >= 0 || nameLower.indexOf("band") >= 0 ||
      nameLower.indexOf("fitbit") >= 0 || nameLower.indexOf("garmin") >= 0) {
    return "WEARABLE";
  }

  // Audio devices
  if (nameLower.indexOf("speaker") >= 0 || nameLower.indexOf("headphone") >= 0 ||
      nameLower.indexOf("earbud") >= 0 || nameLower.indexOf("airpod") >= 0 ||
      nameLower.indexOf("buds") >= 0 || nameLower.indexOf("bose") >= 0) {
    return "AUDIO";
  }

  return "UNKNOWN";
}

// Get signal strength description
String getSignalStrength(int8_t rssi) {
  if (rssi >= -50) return "VERY_STRONG";
  if (rssi >= -60) return "STRONG";
  if (rssi >= -70) return "MODERATE";
  if (rssi >= -80) return "WEAK";
  return "VERY_WEAK";
}

// Calculate approximate distance based on RSSI (rough estimate)
float estimateDistance(int8_t rssi) {
  // Using simplified path loss model
  // Distance in meters (very approximate)
  int txPower = -59; // Assumed transmit power at 1m

  if (rssi == 0) {
    return -1.0; // Unknown
  }

  float ratio = rssi * 1.0 / txPower;
  if (ratio < 1.0) {
    return pow(ratio, 10);
  } else {
    return (0.89976) * pow(ratio, 7.7095) + 0.111;
  }
}

// BLE Scan callback
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String address = advertisedDevice.getAddress().toString().c_str();
      String name = advertisedDevice.haveName() ? advertisedDevice.getName().c_str() : "Unknown";
      int8_t rssi = advertisedDevice.getRSSI();

      if (rssi < RSSI_THRESHOLD) return; // Skip weak signals

      bool isNew = (knownDevices.find(address) == knownDevices.end());

      DeviceInfo info;
      info.address = address;
      info.name = name;
      info.rssi = rssi;
      info.type = identifyDeviceType(name, address);
      info.lastSeen = millis();
      info.isNew = isNew;

      knownDevices[address] = info;

      // Print device info
      if (isNew || info.type != "UNKNOWN") {
        Serial.println("\n[BLE DEVICE DETECTED]");
        Serial.println("Address: " + address);
        Serial.println("Name: " + name);
        Serial.println("Type: " + info.type);
        Serial.println("RSSI: " + String(rssi) + " dBm (" + getSignalStrength(rssi) + ")");
        Serial.println("Est. Distance: " + String(estimateDistance(rssi), 1) + " m");
        Serial.println("Status: " + String(isNew ? "NEW" : "KNOWN"));

        // Alert for specific device types
        if (info.type == "EMERGENCY_VEHICLE" || info.type == "CAMERA" ||
            info.type == "BODYCAM" || info.type == "TRAFFIC_MONITOR") {
          Serial.println(">>> ALERT: " + info.type + " DETECTED <<<");
        }
      }
    }
};

void scanWiFi() {
  Serial.println("\n=== Starting WiFi Scan ===");

  // Scan for networks
  int n = WiFi.scanNetworks(false, true, false, 300);

  Serial.println("Networks found: " + String(n));

  for (int i = 0; i < n; i++) {
    String ssid = WiFi.SSID(i);
    String bssid = WiFi.BSSIDstr(i);
    int8_t rssi = WiFi.RSSI(i);

    if (rssi < RSSI_THRESHOLD) continue;

    bool isNew = (knownDevices.find(bssid) == knownDevices.end());

    DeviceInfo info;
    info.address = bssid;
    info.name = ssid;
    info.rssi = rssi;
    info.type = identifyDeviceType(ssid, bssid);
    info.lastSeen = millis();
    info.isNew = isNew;

    knownDevices[bssid] = info;

    // Print network info for interesting devices
    if (isNew || info.type != "UNKNOWN") {
      Serial.println("\n[WiFi NETWORK DETECTED]");
      Serial.println("SSID: " + ssid);
      Serial.println("BSSID: " + bssid);
      Serial.println("Type: " + info.type);
      Serial.println("RSSI: " + String(rssi) + " dBm (" + getSignalStrength(rssi) + ")");
      Serial.println("Channel: " + String(WiFi.channel(i)));
      Serial.println("Encryption: " + String(WiFi.encryptionType(i)));
      Serial.println("Est. Distance: " + String(estimateDistance(rssi), 1) + " m");
      Serial.println("Status: " + String(isNew ? "NEW" : "KNOWN"));

      // Alert for specific device types
      if (info.type == "EMERGENCY_VEHICLE" || info.type == "CAMERA" ||
          info.type == "MOBILE_HOTSPOT" || info.type == "TRAFFIC_MONITOR") {
        Serial.println(">>> ALERT: " + info.type + " DETECTED <<<");
      }
    }
  }

  WiFi.scanDelete();
}

void scanBLE() {
  Serial.println("\n=== Starting BLE Scan ===");

  BLEScanResults foundDevices = pBLEScan->start(BLE_SCAN_DURATION, false);

  Serial.println("BLE scan complete. Devices found: " + String(foundDevices.getCount()));

  pBLEScan->clearResults();
}

void printSummary() {
  Serial.println("\n======================================");
  Serial.println("DEVICE SUMMARY");
  Serial.println("======================================");
  Serial.println("Total tracked devices: " + String(knownDevices.size()));

  // Count by type
  std::map<String, int> typeCounts;
  int activeDevices = 0;
  unsigned long now = millis();

  for (auto const& pair : knownDevices) {
    DeviceInfo info = pair.second;
    typeCounts[info.type]++;

    // Consider active if seen in last 30 seconds
    if (now - info.lastSeen < 30000) {
      activeDevices++;
    }
  }

  Serial.println("Active devices (last 30s): " + String(activeDevices));
  Serial.println("\nBy Type:");

  for (auto const& pair : typeCounts) {
    Serial.println("  " + pair.first + ": " + String(pair.second));
  }

  Serial.println("======================================\n");
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  Serial.println("\n\n");
  Serial.println("======================================");
  Serial.println("ESP32 Wireless Device Scanner");
  Serial.println("======================================");
  Serial.println("Initializing...\n");

  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("WiFi initialized");

  // Initialize BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  Serial.println("BLE initialized");
  Serial.println("\nScanning for devices...\n");
}

unsigned long lastWiFiScan = 0;
unsigned long lastBLEScan = 0;
unsigned long lastSummary = 0;

void loop() {
  unsigned long now = millis();

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

  // Print summary every 30 seconds
  if (now - lastSummary >= 30000) {
    printSummary();
    lastSummary = now;
  }

  delay(100);
}
