/*
 * ESP32 Basic WiFi/BLE Scanner - Simplified Version
 * Perfect for beginners learning ESP32 wireless scanning
 *
 * This is a minimal version without alerts or complex features
 * Good starting point for understanding the basics
 */

#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SCAN_INTERVAL 10000  // Scan every 10 seconds
BLEScan* pBLEScan;

// BLE callback - prints when BLE device is found
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Device: ");
      Serial.print(advertisedDevice.getAddress().toString().c_str());
      Serial.print(" | ");
      if (advertisedDevice.haveName()) {
        Serial.print(advertisedDevice.getName().c_str());
      } else {
        Serial.print("Unnamed");
      }
      Serial.print(" | RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
    }
};

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nESP32 Basic Scanner");
  Serial.println("==================\n");

  // Initialize WiFi for scanning
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("WiFi initialized");

  // Initialize BLE for scanning
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("BLE initialized\n");
}

void loop() {
  // Scan WiFi networks
  Serial.println("--- WiFi Scan ---");
  int n = WiFi.scanNetworks();
  Serial.println("Found " + String(n) + " networks:");

  for (int i = 0; i < n; i++) {
    Serial.print("  ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
  }
  WiFi.scanDelete();

  // Scan BLE devices
  Serial.println("\n--- BLE Scan ---");
  BLEScanResults results = pBLEScan->start(5, false);
  Serial.println("Found " + String(results.getCount()) + " devices");
  pBLEScan->clearResults();

  Serial.println("\nWaiting...\n");
  delay(SCAN_INTERVAL);
}
