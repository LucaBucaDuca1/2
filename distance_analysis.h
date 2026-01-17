/*
 * Advanced Distance Analysis Module for ESP32 Scanner
 * Provides accurate distance estimation with device-specific calibration
 * and real-time tracking of distance changes
 */

#ifndef DISTANCE_ANALYSIS_H
#define DISTANCE_ANALYSIS_H

#include <Arduino.h>
#include <map>
#include <vector>

// Device-specific TX power calibration (measured power at 1 meter)
// These values are empirically determined for different device types
struct TxPowerProfile {
  const char* deviceType;
  int8_t txPower;       // dBm at 1 meter
  float pathLossExp;    // Path loss exponent (2.0-4.0)
};

const TxPowerProfile txPowerProfiles[] = {
  // High-power devices
  {"CAMERA", -50, 2.5},              // IP cameras often have strong WiFi
  {"ROUTER", -45, 2.7},              // Routers have highest power
  {"MOBILE_HOTSPOT", -50, 2.5},      // Mobile hotspots
  {"TRAFFIC_MONITOR", -48, 2.6},     // Traffic systems

  // Medium-power devices
  {"EMERGENCY_VEHICLE", -55, 2.8},   // Vehicle systems
  {"VEHICLE", -58, 2.9},             // Car Bluetooth
  {"MDT_COMPUTER", -52, 2.7},        // Mobile Data Terminals
  {"COMPUTER", -55, 2.8},            // Laptops
  {"DASHCAM", -57, 2.8},             // Dash cameras

  // Low-power devices (BLE)
  {"SMARTPHONE", -59, 3.0},          // Phones (BLE)
  {"WEARABLE", -65, 3.2},            // Smartwatches (very low power)
  {"BODYCAM", -60, 3.0},             // Body cameras
  {"AUDIO", -62, 3.1},               // Headphones, earbuds
  {"IOT_DEVICE", -63, 3.1},          // IoT sensors

  // Default
  {"UNKNOWN", -59, 3.0}              // Default medium power
};

const int txPowerProfilesSize = sizeof(txPowerProfiles) / sizeof(TxPowerProfile);

// Distance history for tracking trends
#define DISTANCE_HISTORY_SIZE 10

struct DistanceData {
  float currentDistance;
  float smoothedDistance;
  std::vector<float> distanceHistory;
  std::vector<int8_t> rssiHistory;
  unsigned long lastUpdate;
  float velocity;           // Rate of change (m/s)
  String trend;            // "APPROACHING", "RECEDING", "STABLE"
  float minDistance;       // Closest approach
  float maxDistance;       // Farthest detected
  int updateCount;
};

std::map<String, DistanceData> distanceTracker;

// Get TX power for device type
int8_t getTxPower(String deviceType) {
  for (int i = 0; i < txPowerProfilesSize; i++) {
    if (deviceType == String(txPowerProfiles[i].deviceType)) {
      return txPowerProfiles[i].txPower;
    }
  }
  return -59; // Default
}

// Get path loss exponent for device type
float getPathLossExponent(String deviceType) {
  for (int i = 0; i < txPowerProfilesSize; i++) {
    if (deviceType == String(txPowerProfiles[i].deviceType)) {
      return txPowerProfiles[i].pathLossExp;
    }
  }
  return 3.0; // Default
}

// Enhanced distance calculation using Log-Distance Path Loss Model
float calculateDistance(int8_t rssi, String deviceType) {
  if (rssi == 0 || rssi > -10) return -1.0; // Invalid RSSI

  int8_t txPower = getTxPower(deviceType);
  float pathLossExp = getPathLossExponent(deviceType);

  // Log-Distance Path Loss Model:
  // RSSI = TxPower - 10 * n * log10(distance)
  // distance = 10 ^ ((TxPower - RSSI) / (10 * n))

  float ratio = (float)(txPower - rssi) / (10.0 * pathLossExp);
  float distance = pow(10.0, ratio);

  // Sanity checks
  if (distance < 0.1) distance = 0.1;   // Minimum 10cm
  if (distance > 500) distance = 500;   // Maximum 500m

  return distance;
}

// Alternative calculation method using free-space path loss (more accurate for BLE)
float calculateDistanceFreeSpace(int8_t rssi, String deviceType) {
  if (rssi == 0 || rssi > -10) return -1.0;

  int8_t txPower = getTxPower(deviceType);

  // Simplified free-space model
  // Works well for BLE and short ranges
  float ratio = (float)(txPower - rssi) / 20.0;
  float distance = pow(10.0, ratio);

  if (distance < 0.1) distance = 0.1;
  if (distance > 200) distance = 200;

  return distance;
}

// Kalman-like simple filter for smoothing distance readings
float smoothDistance(String address, float newDistance) {
  if (distanceTracker.find(address) == distanceTracker.end()) {
    return newDistance; // First reading
  }

  DistanceData& data = distanceTracker[address];

  // Simple exponential moving average
  // Alpha = 0.3 means 30% new value, 70% old value
  float alpha = 0.3;
  float smoothed = alpha * newDistance + (1.0 - alpha) * data.smoothedDistance;

  return smoothed;
}

// Update distance tracking and calculate trends
void updateDistanceTracking(String address, int8_t rssi, String deviceType) {
  float rawDistance = calculateDistance(rssi, deviceType);
  if (rawDistance < 0) return; // Invalid

  // Get or create tracking data
  DistanceData& data = distanceTracker[address];

  unsigned long now = millis();

  // Initialize on first update
  if (data.updateCount == 0) {
    data.currentDistance = rawDistance;
    data.smoothedDistance = rawDistance;
    data.minDistance = rawDistance;
    data.maxDistance = rawDistance;
    data.velocity = 0;
    data.trend = "STABLE";
    data.lastUpdate = now;
    data.updateCount = 1;
    data.rssiHistory.push_back(rssi);
    data.distanceHistory.push_back(rawDistance);
    return;
  }

  // Calculate time delta
  float timeDelta = (now - data.lastUpdate) / 1000.0; // seconds
  if (timeDelta < 0.1) timeDelta = 0.1; // Minimum

  // Update histories
  data.rssiHistory.push_back(rssi);
  data.distanceHistory.push_back(rawDistance);

  // Keep history size limited
  if (data.rssiHistory.size() > DISTANCE_HISTORY_SIZE) {
    data.rssiHistory.erase(data.rssiHistory.begin());
    data.distanceHistory.erase(data.distanceHistory.begin());
  }

  // Smooth the distance
  float previousSmoothed = data.smoothedDistance;
  data.smoothedDistance = smoothDistance(address, rawDistance);
  data.currentDistance = rawDistance;

  // Calculate velocity (rate of change)
  float distanceChange = data.smoothedDistance - previousSmoothed;
  data.velocity = distanceChange / timeDelta; // meters per second

  // Update min/max
  if (data.smoothedDistance < data.minDistance) {
    data.minDistance = data.smoothedDistance;
  }
  if (data.smoothedDistance > data.maxDistance) {
    data.maxDistance = data.smoothedDistance;
  }

  // Determine trend
  float velocityThreshold = 0.2; // 0.2 m/s threshold for movement detection

  if (data.velocity < -velocityThreshold) {
    data.trend = "APPROACHING";
  } else if (data.velocity > velocityThreshold) {
    data.trend = "RECEDING";
  } else {
    data.trend = "STABLE";
  }

  // Update metadata
  data.lastUpdate = now;
  data.updateCount++;
}

// Get distance info for a device
DistanceData getDistanceInfo(String address) {
  if (distanceTracker.find(address) != distanceTracker.end()) {
    return distanceTracker[address];
  }

  // Return empty data if not found
  DistanceData empty;
  empty.currentDistance = -1;
  empty.smoothedDistance = -1;
  empty.updateCount = 0;
  return empty;
}

// Format distance for display
String formatDistance(float distance) {
  if (distance < 0) return "Unknown";

  if (distance < 1.0) {
    return String(distance * 100, 0) + " cm";
  } else if (distance < 100) {
    return String(distance, 1) + " m";
  } else {
    return String(distance, 0) + " m";
  }
}

// Get distance category
String getDistanceCategory(float distance) {
  if (distance < 0) return "UNKNOWN";
  if (distance < 2.0) return "IMMEDIATE";      // < 2m
  if (distance < 10.0) return "VERY_CLOSE";    // 2-10m
  if (distance < 25.0) return "CLOSE";         // 10-25m
  if (distance < 50.0) return "NEARBY";        // 25-50m
  if (distance < 100.0) return "MODERATE";     // 50-100m
  return "FAR";                                 // > 100m
}

// Get visual indicator for distance
String getDistanceIndicator(float distance) {
  if (distance < 0) return "?";
  if (distance < 2.0) return "🔴";    // Immediate - red dot
  if (distance < 10.0) return "🟠";   // Very close - orange
  if (distance < 25.0) return "🟡";   // Close - yellow
  if (distance < 50.0) return "🟢";   // Nearby - green
  if (distance < 100.0) return "🔵";  // Moderate - blue
  return "⚪";                         // Far - white
}

// Get trend indicator
String getTrendIndicator(String trend) {
  if (trend == "APPROACHING") return "⬇️ APPROACHING";
  if (trend == "RECEDING") return "⬆️ RECEDING";
  if (trend == "STABLE") return "↔️ STABLE";
  return "—";
}

// Get velocity description
String getVelocityDescription(float velocity) {
  float absVel = abs(velocity);

  if (absVel < 0.2) return "Stationary";
  if (absVel < 0.5) return "Slow";
  if (absVel < 1.5) return "Walking";
  if (absVel < 5.0) return "Running";
  if (absVel < 15.0) return "Vehicle";
  return "Fast Vehicle";
}

// Calculate average RSSI from history
float getAverageRSSI(String address) {
  if (distanceTracker.find(address) == distanceTracker.end()) {
    return 0;
  }

  DistanceData& data = distanceTracker[address];
  if (data.rssiHistory.empty()) return 0;

  float sum = 0;
  for (int8_t rssi : data.rssiHistory) {
    sum += rssi;
  }

  return sum / data.rssiHistory.size();
}

// Predict future distance based on velocity
float predictDistance(String address, float seconds) {
  if (distanceTracker.find(address) == distanceTracker.end()) {
    return -1;
  }

  DistanceData& data = distanceTracker[address];
  float predicted = data.smoothedDistance + (data.velocity * seconds);

  if (predicted < 0) predicted = 0;

  return predicted;
}

// Check if device is within proximity threshold
bool isWithinProximity(String address, float thresholdMeters) {
  if (distanceTracker.find(address) == distanceTracker.end()) {
    return false;
  }

  DistanceData& data = distanceTracker[address];
  return data.smoothedDistance <= thresholdMeters;
}

// Get signal quality based on RSSI and distance
String getSignalQuality(int8_t rssi, float distance) {
  // Expected RSSI for the distance
  // Better than expected = good quality
  // Worse than expected = poor quality (obstacles/interference)

  if (distance < 10 && rssi > -60) return "EXCELLENT";
  if (distance < 25 && rssi > -70) return "GOOD";
  if (distance < 50 && rssi > -80) return "FAIR";
  if (rssi > -85) return "POOR";
  return "VERY_POOR";
}

// Clear old tracking data (devices not seen recently)
void cleanupOldDistanceData(unsigned long maxAge) {
  unsigned long now = millis();

  for (auto it = distanceTracker.begin(); it != distanceTracker.end(); ) {
    if (now - it->second.lastUpdate > maxAge) {
      it = distanceTracker.erase(it);
    } else {
      ++it;
    }
  }
}

// Get statistics for all tracked devices
struct DistanceStats {
  int totalDevices;
  int immediate;      // < 2m
  int veryClose;      // 2-10m
  int close;          // 10-25m
  int nearby;         // 25-50m
  int moderate;       // 50-100m
  int far;            // > 100m
  int approaching;
  int receding;
  float closestDistance;
  String closestDevice;
};

DistanceStats getDistanceStatistics() {
  DistanceStats stats;
  stats.totalDevices = 0;
  stats.immediate = 0;
  stats.veryClose = 0;
  stats.close = 0;
  stats.nearby = 0;
  stats.moderate = 0;
  stats.far = 0;
  stats.approaching = 0;
  stats.receding = 0;
  stats.closestDistance = 999999;
  stats.closestDevice = "";

  for (auto const& pair : distanceTracker) {
    DistanceData data = pair.second;
    stats.totalDevices++;

    float dist = data.smoothedDistance;

    // Distance categories
    if (dist < 2.0) stats.immediate++;
    else if (dist < 10.0) stats.veryClose++;
    else if (dist < 25.0) stats.close++;
    else if (dist < 50.0) stats.nearby++;
    else if (dist < 100.0) stats.moderate++;
    else stats.far++;

    // Trends
    if (data.trend == "APPROACHING") stats.approaching++;
    if (data.trend == "RECEDING") stats.receding++;

    // Closest device
    if (dist < stats.closestDistance) {
      stats.closestDistance = dist;
      stats.closestDevice = pair.first;
    }
  }

  return stats;
}

#endif // DISTANCE_ANALYSIS_H
