/*
 * Device identification patterns and MAC address vendor database
 * Helps identify device types based on names, SSIDs, and MAC prefixes
 */

#ifndef DEVICE_PATTERNS_H
#define DEVICE_PATTERNS_H

#include <Arduino.h>

// Common MAC address prefixes (OUI - Organizationally Unique Identifier)
// First 3 bytes of MAC address identify the manufacturer

struct VendorInfo {
  const char* prefix;
  const char* vendor;
  const char* likelyType;
};

const VendorInfo vendorDatabase[] = {
  // Camera manufacturers
  {"00:12:12", "Hikvision", "CAMERA"},
  {"44:19:B6", "Hikvision", "CAMERA"},
  {"BC:AD:28", "Hikvision", "CAMERA"},
  {"00:1F:C1", "Dahua", "CAMERA"},
  {"F4:2A:58", "Dahua", "CAMERA"},
  {"00:40:8C", "Axis Communications", "CAMERA"},
  {"AC:CC:8E", "Axis Communications", "CAMERA"},
  {"B8:A4:4F", "Axis Communications", "CAMERA"},
  {"00:0F:7C", "VIVOTEK", "CAMERA"},
  {"00:02:D1", "VIVOTEK", "CAMERA"},

  // Body camera manufacturers
  {"00:25:DF", "Axon (Taser)", "BODYCAM"},
  {"C0:C5:20", "Axon Enterprise", "BODYCAM"},
  {"00:90:C2", "Panasonic (Toughbook)", "MDT_COMPUTER"},

  // Vehicle entertainment systems
  {"00:0A:30", "Harman (car audio)", "VEHICLE"},
  {"00:17:CA", "Harman", "VEHICLE"},
  {"00:21:FB", "Harman Becker", "VEHICLE"},
  {"00:26:FC", "Panasonic Automotive", "VEHICLE"},

  // Common smartphone/tablet manufacturers
  {"00:50:C2", "Apple", "SMARTPHONE"},
  {"AC:DE:48", "Apple", "SMARTPHONE"},
  {"28:F0:76", "Apple", "SMARTPHONE"},
  {"00:1B:63", "Apple", "SMARTPHONE"},
  {"3C:15:C2", "Samsung", "SMARTPHONE"},
  {"E4:92:FB", "Samsung", "SMARTPHONE"},
  {"28:39:5E", "Samsung", "SMARTPHONE"},

  // IoT and embedded devices
  {"18:FE:34", "Espressif (ESP32)", "IOT_DEVICE"},
  {"24:0A:C4", "Espressif (ESP32)", "IOT_DEVICE"},
  {"30:AE:A4", "Espressif (ESP32)", "IOT_DEVICE"},
  {"B4:E6:2D", "Espressif (ESP8266)", "IOT_DEVICE"},

  // Computer manufacturers (for MDTs)
  {"00:12:3F", "Dell", "COMPUTER"},
  {"00:1E:4F", "Dell", "COMPUTER"},
  {"E0:DB:55", "Dell", "COMPUTER"},
  {"00:13:02", "Panasonic (Toughbook)", "MDT_COMPUTER"},
  {"00:0B:97", "Panasonic", "MDT_COMPUTER"},

  // Wireless routers and hotspots
  {"00:24:01", "Cradlepoint (mobile routers)", "MOBILE_HOTSPOT"},
  {"00:30:44", "Cradlepoint", "MOBILE_HOTSPOT"},
  {"A0:04:60", "Netgear", "ROUTER"},
  {"E0:46:9A", "Netgear", "ROUTER"},

  // Dash cameras
  {"00:1D:43", "Garmin", "DASHCAM"},
  {"00:87:10", "Garmin", "DASHCAM"},
  {"88:C9:D0", "Garmin", "DASHCAM"},

  // Motorola (public safety radios, computers)
  {"00:04:BD", "Motorola", "RADIO"},
  {"00:0C:E7", "Motorola", "RADIO"},
  {"00:1A:77", "Motorola", "RADIO"},
  {"00:90:9C", "Motorola (Public Safety)", "RADIO"},
};

const int vendorDatabaseSize = sizeof(vendorDatabase) / sizeof(VendorInfo);

// Name-based detection patterns
struct NamePattern {
  const char* keyword;
  const char* deviceType;
  bool caseSensitive;
};

const NamePattern namePatterns[] = {
  // Emergency services
  {"police", "EMERGENCY_VEHICLE", false},
  {"sheriff", "EMERGENCY_VEHICLE", false},
  {"patrol", "EMERGENCY_VEHICLE", false},
  {"squad", "EMERGENCY_VEHICLE", false},
  {"trooper", "EMERGENCY_VEHICLE", false},
  {"state police", "EMERGENCY_VEHICLE", false},
  {"ambulance", "EMERGENCY_VEHICLE", false},
  {"ems", "EMERGENCY_VEHICLE", false},
  {"fire", "EMERGENCY_VEHICLE", false},
  {"rescue", "EMERGENCY_VEHICLE", false},

  // Camera systems
  {"camera", "CAMERA", false},
  {"ipcam", "CAMERA", false},
  {"webcam", "CAMERA", false},
  {"cam", "CAMERA", false},
  {"nvr", "CAMERA", false},
  {"dvr", "CAMERA", false},
  {"cctv", "CAMERA", false},
  {"surveillance", "CAMERA", false},
  {"hikvision", "CAMERA", false},
  {"dahua", "CAMERA", false},
  {"axis", "CAMERA", false},
  {"vivotek", "CAMERA", false},

  // Dash cameras
  {"dashcam", "DASHCAM", false},
  {"dash cam", "DASHCAM", false},
  {"viofo", "DASHCAM", false},
  {"nextbase", "DASHCAM", false},
  {"blackvue", "DASHCAM", false},

  // Body cameras
  {"bodycam", "BODYCAM", false},
  {"body cam", "BODYCAM", false},
  {"bodyworn", "BODYCAM", false},
  {"axon", "BODYCAM", false},
  {"watchguard", "BODYCAM", false},

  // Vehicles
  {"ford", "VEHICLE", false},
  {"chevy", "VEHICLE", false},
  {"chevrolet", "VEHICLE", false},
  {"dodge", "VEHICLE", false},
  {"toyota", "VEHICLE", false},
  {"honda", "VEHICLE", false},
  {"nissan", "VEHICLE", false},
  {"bmw", "VEHICLE", false},
  {"mercedes", "VEHICLE", false},
  {"audi", "VEHICLE", false},
  {"tesla", "VEHICLE", false},
  {"sync", "VEHICLE", false},
  {"mylink", "VEHICLE", false},
  {"uconnect", "VEHICLE", false},
  {"carplay", "VEHICLE", false},
  {"android auto", "VEHICLE", false},

  // Mobile hotspots and MDTs
  {"hotspot", "MOBILE_HOTSPOT", false},
  {"mdt", "MDT_COMPUTER", false},
  {"mobile data", "MDT_COMPUTER", false},
  {"cruiser", "MDT_COMPUTER", false},
  {"unit", "MDT_COMPUTER", false},
  {"car-pc", "MDT_COMPUTER", false},

  // Traffic systems
  {"traffic", "TRAFFIC_MONITOR", false},
  {"sensor", "TRAFFIC_MONITOR", false},
  {"radar", "TRAFFIC_MONITOR", false},

  // Radios
  {"radio", "RADIO", false},
  {"motorola", "RADIO", false},
  {"kenwood", "RADIO", false},
  {"icom", "RADIO", false},

  // Smartphones
  {"iphone", "SMARTPHONE", false},
  {"android", "SMARTPHONE", false},
  {"samsung", "SMARTPHONE", false},
  {"galaxy", "SMARTPHONE", false},
  {"pixel", "SMARTPHONE", false},
  {"oneplus", "SMARTPHONE", false},
  {"xiaomi", "SMARTPHONE", false},

  // Computers
  {"laptop", "COMPUTER", false},
  {"macbook", "COMPUTER", false},
  {"dell", "COMPUTER", false},
  {"hp", "COMPUTER", false},
  {"lenovo", "COMPUTER", false},
  {"thinkpad", "COMPUTER", false},
  {"toughbook", "MDT_COMPUTER", false},

  // Wearables
  {"watch", "WEARABLE", false},
  {"fitbit", "WEARABLE", false},
  {"garmin", "WEARABLE", false},
  {"apple watch", "WEARABLE", false},

  // Audio
  {"speaker", "AUDIO", false},
  {"headphone", "AUDIO", false},
  {"headset", "AUDIO", false},
  {"earbud", "AUDIO", false},
  {"airpod", "AUDIO", false},
  {"buds", "AUDIO", false},
  {"bose", "AUDIO", false},
  {"beats", "AUDIO", false},
};

const int namePatternsSize = sizeof(namePatterns) / sizeof(NamePattern);

// Helper function to lookup vendor by MAC prefix
String lookupVendor(String macAddress) {
  // Extract first 3 bytes (6 characters + 2 colons)
  if (macAddress.length() < 8) return "Unknown";

  String prefix = macAddress.substring(0, 8);
  prefix.toUpperCase();

  for (int i = 0; i < vendorDatabaseSize; i++) {
    String dbPrefix = String(vendorDatabase[i].prefix);
    dbPrefix.toUpperCase();

    if (prefix == dbPrefix) {
      return String(vendorDatabase[i].vendor);
    }
  }

  return "Unknown";
}

// Helper function to get likely device type from MAC address
String getTypeFromMAC(String macAddress) {
  if (macAddress.length() < 8) return "UNKNOWN";

  String prefix = macAddress.substring(0, 8);
  prefix.toUpperCase();

  for (int i = 0; i < vendorDatabaseSize; i++) {
    String dbPrefix = String(vendorDatabase[i].prefix);
    dbPrefix.toUpperCase();

    if (prefix == dbPrefix) {
      return String(vendorDatabase[i].likelyType);
    }
  }

  return "UNKNOWN";
}

#endif // DEVICE_PATTERNS_H
