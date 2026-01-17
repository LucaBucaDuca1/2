/*
 * ENHANCED Device identification patterns and MAC address vendor database
 * Expanded for LA County and Southern California government devices
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
  // ==================== CAMERA MANUFACTURERS ====================

  // Hikvision (very common in government installations)
  {"00:12:12", "Hikvision", "CAMERA"},
  {"44:19:B6", "Hikvision", "CAMERA"},
  {"BC:AD:28", "Hikvision", "CAMERA"},
  {"C4:2F:90", "Hikvision", "CAMERA"},
  {"28:57:BE", "Hikvision", "CAMERA"},
  {"54:C4:15", "Hikvision", "CAMERA"},

  // Dahua (surveillance cameras)
  {"00:1F:C1", "Dahua", "CAMERA"},
  {"F4:2A:58", "Dahua", "CAMERA"},
  {"6C:7E:67", "Dahua", "CAMERA"},
  {"A4:14:37", "Dahua", "CAMERA"},

  // Axis Communications (high-end surveillance)
  {"00:40:8C", "Axis Communications", "CAMERA"},
  {"AC:CC:8E", "Axis Communications", "CAMERA"},
  {"B8:A4:4F", "Axis Communications", "CAMERA"},
  {"00:09:5B", "Axis Communications", "CAMERA"},

  // VIVOTEK (IP cameras)
  {"00:0F:7C", "VIVOTEK", "CAMERA"},
  {"00:02:D1", "VIVOTEK", "CAMERA"},

  // Hanwha (Samsung) Techwin
  {"00:09:18", "Hanwha Techwin", "CAMERA"},
  {"A0:1D:48", "Hanwha Techwin", "CAMERA"},

  // Bosch Security Systems
  {"00:12:E0", "Bosch Security", "CAMERA"},
  {"00:1C:06", "Bosch Security", "CAMERA"},

  // Avigilon (high-end security)
  {"00:18:85", "Avigilon", "CAMERA"},
  {"F4:7B:5E", "Avigilon", "CAMERA"},

  // Pelco (security cameras)
  {"00:03:C5", "Pelco", "CAMERA"},
  {"00:E0:7D", "Pelco", "CAMERA"},

  // Genetec (security management)
  {"00:0C:42", "Genetec", "CAMERA"},

  // Milestone Systems
  {"00:1B:C5", "Milestone Systems", "CAMERA"},

  // Mobotix (German cameras)
  {"00:03:C5", "Mobotix", "CAMERA"},

  // ==================== LICENSE PLATE READERS (ALPR) ====================

  // Vigilant Solutions (ALPR systems)
  {"00:1D:0F", "Vigilant Solutions", "ALPR"},
  {"00:25:90", "Vigilant Solutions", "ALPR"},

  // ELSAG (Leonardo - ALPR)
  {"00:50:C2", "ELSAG ALPR", "ALPR"},

  // Genetec AutoVu
  {"00:0F:EA", "Genetec AutoVu", "ALPR"},

  // Neology (RFID/ALPR)
  {"00:1C:23", "Neology", "ALPR"},

  // ==================== BODY CAMERAS ====================

  // Axon (Taser) - very common with LAPD, LASD
  {"00:25:DF", "Axon (Taser)", "BODYCAM"},
  {"C0:C5:20", "Axon Enterprise", "BODYCAM"},
  {"00:40:9D", "Axon", "BODYCAM"},
  {"D4:6A:6A", "Axon", "BODYCAM"},

  // WatchGuard/Motorola Solutions
  {"00:0D:28", "WatchGuard", "BODYCAM"},
  {"00:1A:8C", "WatchGuard", "BODYCAM"},

  // Panasonic Arbitrator (body cams)
  {"00:80:F0", "Panasonic Arbitrator", "BODYCAM"},

  // Digital Ally (body cams)
  {"00:1F:A7", "Digital Ally", "BODYCAM"},

  // Reveal Media (body cams)
  {"00:1E:C0", "Reveal Media", "BODYCAM"},

  // ==================== MOBILE DATA TERMINALS (MDTs) ====================

  // Panasonic Toughbook (extremely common in police vehicles)
  {"00:90:C2", "Panasonic Toughbook", "MDT_COMPUTER"},
  {"00:13:02", "Panasonic Toughbook", "MDT_COMPUTER"},
  {"00:0B:97", "Panasonic", "MDT_COMPUTER"},
  {"08:3E:8E", "Panasonic", "MDT_COMPUTER"},
  {"04:4B:ED", "Panasonic", "MDT_COMPUTER"},

  // Dell (Latitude Rugged for law enforcement)
  {"00:12:3F", "Dell", "COMPUTER"},
  {"00:1E:4F", "Dell", "COMPUTER"},
  {"E0:DB:55", "Dell", "COMPUTER"},
  {"D4:BE:D9", "Dell", "COMPUTER"},
  {"18:03:73", "Dell", "COMPUTER"},

  // Getac (rugged computers)
  {"00:13:E8", "Getac", "MDT_COMPUTER"},
  {"00:90:F8", "Getac", "MDT_COMPUTER"},

  // Zebra Technologies (rugged tablets)
  {"00:A0:F8", "Zebra Technologies", "MDT_COMPUTER"},
  {"00:07:4D", "Zebra Technologies", "MDT_COMPUTER"},

  // ==================== MOBILE ROUTERS & HOTSPOTS ====================

  // Cradlepoint (very common in police/fire vehicles)
  {"00:24:01", "Cradlepoint", "MOBILE_HOTSPOT"},
  {"00:30:44", "Cradlepoint", "MOBILE_HOTSPOT"},
  {"F4:E3:FB", "Cradlepoint", "MOBILE_HOTSPOT"},
  {"00:1F:CC", "Cradlepoint", "MOBILE_HOTSPOT"},

  // Sierra Wireless (mobile routers)
  {"00:A0:D5", "Sierra Wireless", "MOBILE_HOTSPOT"},
  {"00:1E:67", "Sierra Wireless", "MOBILE_HOTSPOT"},
  {"B0:D5:CC", "Sierra Wireless", "MOBILE_HOTSPOT"},

  // Pepwave/Peplink (MAX routers)
  {"00:30:E4", "Pepwave", "MOBILE_HOTSPOT"},
  {"18:E8:29", "Peplink", "MOBILE_HOTSPOT"},

  // Netgear (mobile hotspots)
  {"A0:04:60", "Netgear", "ROUTER"},
  {"E0:46:9A", "Netgear", "ROUTER"},
  {"84:1B:5E", "Netgear", "ROUTER"},

  // ==================== PUBLIC SAFETY RADIOS ====================

  // Motorola Solutions (P25 radios, APX series)
  {"00:04:BD", "Motorola", "RADIO"},
  {"00:0C:E7", "Motorola", "RADIO"},
  {"00:1A:77", "Motorola", "RADIO"},
  {"00:90:9C", "Motorola Public Safety", "RADIO"},
  {"C0:8A:DE", "Motorola Solutions", "RADIO"},
  {"00:15:9A", "Motorola", "RADIO"},
  {"D4:76:EA", "Motorola", "RADIO"},

  // Harris (L3Harris) - P25 radios
  {"00:A0:64", "Harris RF", "RADIO"},
  {"00:15:77", "Harris", "RADIO"},

  // Kenwood (NX series radios)
  {"00:11:88", "Kenwood", "RADIO"},
  {"00:26:66", "Kenwood", "RADIO"},

  // Icom (public safety)
  {"00:1F:12", "Icom", "RADIO"},
  {"00:A0:57", "Icom", "RADIO"},

  // Tait Communications
  {"00:0C:31", "Tait", "RADIO"},

  // ==================== TRAFFIC MONITORING ====================

  // Sensys Networks (traffic sensors)
  {"00:13:E2", "Sensys Networks", "TRAFFIC_MONITOR"},

  // Wavetronix (SmartSensor, radar)
  {"00:1C:C4", "Wavetronix", "TRAFFIC_MONITOR"},

  // FLIR (traffic cameras)
  {"00:1A:C1", "FLIR Systems", "TRAFFIC_MONITOR"},

  // Iteris (Vantage detection)
  {"00:0E:8C", "Iteris", "TRAFFIC_MONITOR"},

  // Econolite (traffic controllers)
  {"00:80:37", "Econolite", "TRAFFIC_MONITOR"},

  // Trafficware (signal systems)
  {"00:60:6E", "Trafficware", "TRAFFIC_MONITOR"},

  // ==================== DASH CAMERAS ====================

  // Garmin (police dash cams)
  {"00:1D:43", "Garmin", "DASHCAM"},
  {"00:87:10", "Garmin", "DASHCAM"},
  {"88:C9:D0", "Garmin", "DASHCAM"},
  {"C8:90:67", "Garmin", "DASHCAM"},

  // VIOFO
  {"00:26:08", "VIOFO", "DASHCAM"},

  // BlackVue
  {"00:16:6C", "BlackVue", "DASHCAM"},

  // Nextbase
  {"00:1B:63", "Nextbase", "DASHCAM"},

  // ==================== VEHICLE SYSTEMS ====================

  // Harman (car audio/infotainment)
  {"00:0A:30", "Harman", "VEHICLE"},
  {"00:17:CA", "Harman", "VEHICLE"},
  {"00:21:FB", "Harman Becker", "VEHICLE"},

  // Panasonic Automotive
  {"00:26:FC", "Panasonic Automotive", "VEHICLE"},

  // Code 3 (emergency vehicle equipment)
  {"00:0D:B9", "Code 3", "EMERGENCY_EQUIPMENT"},

  // Federal Signal (emergency equipment)
  {"00:50:C2", "Federal Signal", "EMERGENCY_EQUIPMENT"},

  // Whelen (emergency lights/sirens)
  {"00:1B:C5", "Whelen Engineering", "EMERGENCY_EQUIPMENT"},

  // ==================== MESH/TACTICAL NETWORKS ====================

  // Rajant (mesh networks used by emergency services)
  {"00:A0:C8", "Rajant", "MESH_NETWORK"},

  // Persistent Systems (Wave Relay)
  {"00:1C:C0", "Persistent Systems", "MESH_NETWORK"},

  // Silvus Technologies (tactical radios)
  {"00:1E:8C", "Silvus", "MESH_NETWORK"},

  // ==================== COMMON DEVICES ====================

  // Apple (iPhones, iPads common with officers)
  {"00:50:C2", "Apple", "SMARTPHONE"},
  {"AC:DE:48", "Apple", "SMARTPHONE"},
  {"28:F0:76", "Apple", "SMARTPHONE"},
  {"00:1B:63", "Apple", "SMARTPHONE"},
  {"D0:23:DB", "Apple", "SMARTPHONE"},
  {"B8:78:2E", "Apple", "SMARTPHONE"},

  // Samsung (Galaxy phones/tablets)
  {"3C:15:C2", "Samsung", "SMARTPHONE"},
  {"E4:92:FB", "Samsung", "SMARTPHONE"},
  {"28:39:5E", "Samsung", "SMARTPHONE"},
  {"BC:F5:AC", "Samsung", "SMARTPHONE"},

  // Cisco (network infrastructure)
  {"00:1E:14", "Cisco", "NETWORK"},
  {"00:62:EC", "Cisco", "NETWORK"},

  // Ubiquiti (wireless infrastructure)
  {"00:15:6D", "Ubiquiti", "NETWORK"},
  {"04:18:D6", "Ubiquiti", "NETWORK"},

  // IoT and embedded devices
  {"18:FE:34", "Espressif ESP32", "IOT_DEVICE"},
  {"24:0A:C4", "Espressif ESP32", "IOT_DEVICE"},
  {"30:AE:A4", "Espressif ESP32", "IOT_DEVICE"},
  {"B4:E6:2D", "Espressif ESP8266", "IOT_DEVICE"},
};

const int vendorDatabaseSize = sizeof(vendorDatabase) / sizeof(VendorInfo);

// Name-based detection patterns
struct NamePattern {
  const char* keyword;
  const char* deviceType;
  bool caseSensitive;
};

const NamePattern namePatterns[] = {
  // ==================== LA COUNTY AGENCIES ====================

  // Los Angeles Police Department (LAPD)
  {"lapd", "EMERGENCY_VEHICLE", false},
  {"la police", "EMERGENCY_VEHICLE", false},
  {"los angeles police", "EMERGENCY_VEHICLE", false},
  {"lapdair", "EMERGENCY_VEHICLE", false},  // LAPD Air Support
  {"metro", "EMERGENCY_VEHICLE", false},     // LAPD Metro

  // Los Angeles County Sheriff (LASD)
  {"lasd", "EMERGENCY_VEHICLE", false},
  {"la sheriff", "EMERGENCY_VEHICLE", false},
  {"los angeles sheriff", "EMERGENCY_VEHICLE", false},
  {"lacosd", "EMERGENCY_VEHICLE", false},
  {"sheriff star", "EMERGENCY_VEHICLE", false},

  // California Highway Patrol (CHP)
  {"chp", "EMERGENCY_VEHICLE", false},
  {"highway patrol", "EMERGENCY_VEHICLE", false},
  {"california highway", "EMERGENCY_VEHICLE", false},

  // Los Angeles Fire Department (LAFD)
  {"lafd", "EMERGENCY_VEHICLE", false},
  {"la fire", "EMERGENCY_VEHICLE", false},
  {"los angeles fire", "EMERGENCY_VEHICLE", false},

  // LA County Fire
  {"lacfd", "EMERGENCY_VEHICLE", false},
  {"la county fire", "EMERGENCY_VEHICLE", false},

  // Other LA Area Agencies
  {"burbank pd", "EMERGENCY_VEHICLE", false},
  {"glendale pd", "EMERGENCY_VEHICLE", false},
  {"pasadena pd", "EMERGENCY_VEHICLE", false},
  {"long beach pd", "EMERGENCY_VEHICLE", false},
  {"santa monica pd", "EMERGENCY_VEHICLE", false},
  {"beverly hills pd", "EMERGENCY_VEHICLE", false},
  {"torrance pd", "EMERGENCY_VEHICLE", false},
  {"inglewood pd", "EMERGENCY_VEHICLE", false},
  {"culver city pd", "EMERGENCY_VEHICLE", false},
  {"el segundo pd", "EMERGENCY_VEHICLE", false},
  {"redondo beach pd", "EMERGENCY_VEHICLE", false},
  {"manhattan beach pd", "EMERGENCY_VEHICLE", false},
  {"hermosa beach pd", "EMERGENCY_VEHICLE", false},

  // LA Metro/Transit Police
  {"metro transit", "EMERGENCY_VEHICLE", false},
  {"lamta", "EMERGENCY_VEHICLE", false},
  {"metro police", "EMERGENCY_VEHICLE", false},

  // LA Port Police
  {"port police", "EMERGENCY_VEHICLE", false},
  {"lapd port", "EMERGENCY_VEHICLE", false},

  // LA Airport Police
  {"lax pd", "EMERGENCY_VEHICLE", false},
  {"airport police", "EMERGENCY_VEHICLE", false},

  // LA School Police
  {"lausd pd", "EMERGENCY_VEHICLE", false},
  {"school police", "EMERGENCY_VEHICLE", false},

  // ==================== GENERAL EMERGENCY SERVICES ====================

  {"police", "EMERGENCY_VEHICLE", false},
  {"sheriff", "EMERGENCY_VEHICLE", false},
  {"patrol", "EMERGENCY_VEHICLE", false},
  {"squad", "EMERGENCY_VEHICLE", false},
  {"cruiser", "EMERGENCY_VEHICLE", false},
  {"trooper", "EMERGENCY_VEHICLE", false},
  {"state police", "EMERGENCY_VEHICLE", false},
  {"deputy", "EMERGENCY_VEHICLE", false},
  {"officer", "EMERGENCY_VEHICLE", false},

  // Fire/EMS
  {"fire dept", "EMERGENCY_VEHICLE", false},
  {"fire", "EMERGENCY_VEHICLE", false},
  {"ambulance", "EMERGENCY_VEHICLE", false},
  {"ems", "EMERGENCY_VEHICLE", false},
  {"paramedic", "EMERGENCY_VEHICLE", false},
  {"rescue", "EMERGENCY_VEHICLE", false},
  {"medic", "EMERGENCY_VEHICLE", false},

  // Special Units
  {"swat", "EMERGENCY_VEHICLE", false},
  {"k9", "EMERGENCY_VEHICLE", false},
  {"k-9", "EMERGENCY_VEHICLE", false},
  {"detective", "EMERGENCY_VEHICLE", false},
  {"sergeant", "EMERGENCY_VEHICLE", false},
  {"captain", "EMERGENCY_VEHICLE", false},
  {"lieutenant", "EMERGENCY_VEHICLE", false},
  {"chief", "EMERGENCY_VEHICLE", false},
  {"command", "EMERGENCY_VEHICLE", false},

  // ==================== MUNICIPAL/GOVERNMENT ====================

  // Department of Transportation
  {"dot", "MUNICIPAL", false},
  {"caltrans", "MUNICIPAL", false},
  {"ladot", "MUNICIPAL", false},
  {"la dot", "MUNICIPAL", false},
  {"transportation", "MUNICIPAL", false},

  // Parking Enforcement
  {"parking enforcement", "MUNICIPAL", false},
  {"parking authority", "MUNICIPAL", false},
  {"meter maid", "MUNICIPAL", false},

  // City Vehicles
  {"city of la", "MUNICIPAL", false},
  {"city vehicle", "MUNICIPAL", false},
  {"municipal", "MUNICIPAL", false},
  {"public works", "MUNICIPAL", false},
  {"sanitation", "MUNICIPAL", false},

  // ==================== CAMERA SYSTEMS ====================

  {"camera", "CAMERA", false},
  {"ipcam", "CAMERA", false},
  {"webcam", "CAMERA", false},
  {"cam", "CAMERA", false},
  {"nvr", "CAMERA", false},
  {"dvr", "CAMERA", false},
  {"cctv", "CAMERA", false},
  {"surveillance", "CAMERA", false},
  {"security cam", "CAMERA", false},

  // Specific manufacturers
  {"hikvision", "CAMERA", false},
  {"dahua", "CAMERA", false},
  {"axis", "CAMERA", false},
  {"vivotek", "CAMERA", false},
  {"avigilon", "CAMERA", false},
  {"pelco", "CAMERA", false},
  {"bosch", "CAMERA", false},
  {"hanwha", "CAMERA", false},

  // Public cameras
  {"street cam", "CAMERA", false},
  {"city cam", "CAMERA", false},
  {"public cam", "CAMERA", false},
  {"traffic cam", "CAMERA", false},

  // ==================== LICENSE PLATE READERS ====================

  {"alpr", "ALPR", false},
  {"lpr", "ALPR", false},
  {"plate reader", "ALPR", false},
  {"license plate", "ALPR", false},
  {"vigilant", "ALPR", false},
  {"elsag", "ALPR", false},
  {"autovu", "ALPR", false},
  {"pips", "ALPR", false},  // Plate Impact Patrol System

  // ==================== BODY CAMERAS ====================

  {"bodycam", "BODYCAM", false},
  {"body cam", "BODYCAM", false},
  {"bodyworn", "BODYCAM", false},
  {"body worn", "BODYCAM", false},
  {"axon", "BODYCAM", false},
  {"taser", "BODYCAM", false},
  {"watchguard", "BODYCAM", false},
  {"arbitrator", "BODYCAM", false},
  {"reveal", "BODYCAM", false},
  {"bwc", "BODYCAM", false},  // Body Worn Camera

  // ==================== DASH CAMERAS ====================

  {"dashcam", "DASHCAM", false},
  {"dash cam", "DASHCAM", false},
  {"patrol cam", "DASHCAM", false},
  {"car cam", "DASHCAM", false},
  {"vehicle cam", "DASHCAM", false},
  {"viofo", "DASHCAM", false},
  {"nextbase", "DASHCAM", false},
  {"blackvue", "DASHCAM", false},
  {"garmin", "DASHCAM", false},

  // ==================== MOBILE DATA TERMINALS ====================

  {"mdt", "MDT_COMPUTER", false},
  {"mobile data", "MDT_COMPUTER", false},
  {"toughbook", "MDT_COMPUTER", false},
  {"rugged", "MDT_COMPUTER", false},
  {"panasonic cf", "MDT_COMPUTER", false},
  {"getac", "MDT_COMPUTER", false},
  {"patrol laptop", "MDT_COMPUTER", false},
  {"vehicle computer", "MDT_COMPUTER", false},
  {"car-pc", "MDT_COMPUTER", false},

  // Unit identifiers
  {"unit-", "MDT_COMPUTER", false},
  {"unit ", "MDT_COMPUTER", false},
  {"car-", "MDT_COMPUTER", false},
  {"car ", "MDT_COMPUTER", false},

  // ==================== TRAFFIC MONITORING ====================

  {"traffic", "TRAFFIC_MONITOR", false},
  {"sensor", "TRAFFIC_MONITOR", false},
  {"radar", "TRAFFIC_MONITOR", false},
  {"lidar", "TRAFFIC_MONITOR", false},
  {"speed", "TRAFFIC_MONITOR", false},
  {"red light", "TRAFFIC_MONITOR", false},
  {"intersection", "TRAFFIC_MONITOR", false},
  {"signal", "TRAFFIC_MONITOR", false},
  {"smartsensor", "TRAFFIC_MONITOR", false},
  {"wavetronix", "TRAFFIC_MONITOR", false},
  {"iteris", "TRAFFIC_MONITOR", false},
  {"flir", "TRAFFIC_MONITOR", false},

  // ==================== PUBLIC SAFETY RADIOS ====================

  {"radio", "RADIO", false},
  {"motorola apx", "RADIO", false},
  {"p25", "RADIO", false},
  {"kenwood", "RADIO", false},
  {"icom", "RADIO", false},
  {"harris", "RADIO", false},
  {"portable", "RADIO", false},
  {"handheld", "RADIO", false},

  // ==================== VEHICLES ====================

  // Police vehicle brands
  {"ford explorer", "VEHICLE", false},
  {"ford interceptor", "VEHICLE", false},
  {"dodge charger", "VEHICLE", false},
  {"chevy tahoe", "VEHICLE", false},
  {"chevy suburban", "VEHICLE", false},

  // General vehicles
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

  // Infotainment
  {"sync", "VEHICLE", false},
  {"mylink", "VEHICLE", false},
  {"uconnect", "VEHICLE", false},
  {"carplay", "VEHICLE", false},
  {"android auto", "VEHICLE", false},

  // ==================== MOBILE HOTSPOTS ====================

  {"hotspot", "MOBILE_HOTSPOT", false},
  {"cradlepoint", "MOBILE_HOTSPOT", false},
  {"sierra wireless", "MOBILE_HOTSPOT", false},
  {"pepwave", "MOBILE_HOTSPOT", false},
  {"mobile router", "MOBILE_HOTSPOT", false},
  {"vehicle wifi", "MOBILE_HOTSPOT", false},
  {"car wifi", "MOBILE_HOTSPOT", false},

  // ==================== SMARTPHONES ====================

  {"iphone", "SMARTPHONE", false},
  {"android", "SMARTPHONE", false},
  {"samsung", "SMARTPHONE", false},
  {"galaxy", "SMARTPHONE", false},
  {"pixel", "SMARTPHONE", false},
  {"oneplus", "SMARTPHONE", false},
  {"xiaomi", "SMARTPHONE", false},

  // ==================== COMPUTERS ====================

  {"laptop", "COMPUTER", false},
  {"macbook", "COMPUTER", false},
  {"dell", "COMPUTER", false},
  {"hp", "COMPUTER", false},
  {"lenovo", "COMPUTER", false},
  {"thinkpad", "COMPUTER", false},

  // ==================== WEARABLES ====================

  {"watch", "WEARABLE", false},
  {"fitbit", "WEARABLE", false},
  {"apple watch", "WEARABLE", false},

  // ==================== AUDIO ====================

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
