# Distance Analysis System

## Overview

The ESP32 Live Distance Tracker includes an advanced distance estimation and tracking system that provides real-time information about how far away detected devices are and whether they're moving closer or farther away.

## How It Works

### 1. Device-Specific Calibration

Different devices transmit at different power levels. The system uses calibrated TX power profiles for accurate distance estimation:

| Device Type | TX Power @ 1m | Path Loss Exponent | Typical Range |
|-------------|---------------|-------------------|---------------|
| Router | -45 dBm | 2.7 | 100-200m |
| Camera | -50 dBm | 2.5 | 75-150m |
| Mobile Hotspot | -50 dBm | 2.5 | 75-150m |
| Emergency Vehicle | -55 dBm | 2.8 | 50-100m |
| Computer/Laptop | -55 dBm | 2.8 | 50-100m |
| Vehicle Bluetooth | -58 dBm | 2.9 | 30-75m |
| Smartphone | -59 dBm | 3.0 | 25-50m |
| Body Camera | -60 dBm | 3.0 | 25-50m |
| Audio Device | -62 dBm | 3.1 | 15-30m |
| IoT Device | -63 dBm | 3.1 | 15-30m |
| Wearable | -65 dBm | 3.2 | 10-20m |

### 2. Distance Calculation Methods

The system uses the **Log-Distance Path Loss Model**:

```
RSSI = TxPower - 10 × n × log10(distance)

Where:
- RSSI = Received Signal Strength Indicator (measured)
- TxPower = Transmit power at 1 meter (calibrated per device type)
- n = Path loss exponent (2.0 for free space, 2.5-4.0 for indoor/obstacles)
- distance = Distance in meters (what we're calculating)

Solving for distance:
distance = 10 ^ ((TxPower - RSSI) / (10 × n))
```

**Example Calculation:**
```
Device: Smartphone (TxPower = -59 dBm, n = 3.0)
RSSI: -75 dBm

distance = 10 ^ ((-59 - (-75)) / (10 × 3.0))
distance = 10 ^ (16 / 30)
distance = 10 ^ 0.533
distance ≈ 3.4 meters
```

### 3. Signal Smoothing

Raw RSSI readings fluctuate due to:
- Interference
- Multipath propagation
- Device movement
- Environmental factors

The system uses **Exponential Moving Average (EMA)** to smooth readings:

```
SmoothedDistance = 0.3 × NewDistance + 0.7 × PreviousSmoothed
```

This gives 30% weight to new readings and 70% to historical data, reducing noise while staying responsive to changes.

### 4. Movement Detection

The system tracks distance over time to detect movement:

**Velocity Calculation:**
```
Velocity = (Current Distance - Previous Distance) / Time Elapsed
```

**Trend Classification:**
- **APPROACHING**: Velocity < -0.2 m/s (getting closer)
- **RECEDING**: Velocity > +0.2 m/s (moving away)
- **STABLE**: -0.2 ≤ Velocity ≤ +0.2 m/s (stationary)

**Velocity Categories:**
| Velocity | Category | Typical Scenario |
|----------|----------|------------------|
| < 0.2 m/s | Stationary | Person standing still |
| 0.2-0.5 m/s | Slow | Very slow walking |
| 0.5-1.5 m/s | Walking | Normal walking speed |
| 1.5-5.0 m/s | Running | Jogging or running |
| 5.0-15.0 m/s | Vehicle | Car driving (18-54 km/h) |
| > 15.0 m/s | Fast Vehicle | Highway speeds (> 54 km/h) |

### 5. Distance Categories

| Category | Distance Range | Indicator | Description |
|----------|---------------|-----------|-------------|
| IMMEDIATE | < 2m | 🔴 | Extremely close - within arm's reach |
| VERY_CLOSE | 2-10m | 🟠 | Same room / very near |
| CLOSE | 10-25m | 🟡 | Nearby / across street |
| NEARBY | 25-50m | 🟢 | Short distance / neighboring building |
| MODERATE | 50-100m | 🔵 | Medium distance / down the block |
| FAR | > 100m | ⚪ | Long distance / several blocks |

## Features

### Real-Time Tracking

The system maintains a history of up to 10 distance measurements per device, allowing it to:

1. **Smooth out fluctuations** using moving averages
2. **Detect trends** (approaching/receding)
3. **Calculate velocity** (speed of approach/retreat)
4. **Predict future position** based on current velocity
5. **Track min/max distances** observed

### Live Distance Display

Every 5 seconds, the system displays a live update of all tracked devices:

```
[LIVE] 🟡 ⬇️ John's iPhone @ 15.3 m (-0.8 m/s)
[LIVE] 🟢 ↔️ NETGEAR Router @ 42.1 m (0.0 m/s)
[LIVE] 🔴 ⬇️ Squad_Car_23 @ 1.8 m (-1.2 m/s)
```

Format:
- **Indicator**: Color-coded distance category
- **Trend Arrow**: ⬇️ (approaching), ⬆️ (receding), ↔️ (stable)
- **Device Name**: First 20 characters
- **Distance**: In meters or centimeters
- **Velocity**: Rate of change in m/s

### Proximity Alerts

The system triggers alerts based on:

1. **Distance Thresholds:**
   - IMMEDIATE (< 2m): 5 rapid beeps
   - CLOSE (< 5m): 3 beeps
   - PROXIMITY (< 10m): 1-2 beeps

2. **Device Types:**
   - Emergency vehicles: Alert at any distance
   - Body cameras: Alert at any distance
   - Cameras: Alert when < 10m
   - Traffic monitors: Alert when < 50m

### Detailed Device Reports

When a device is detected, the system displays comprehensive information:

```
╔════════════════════════════════════════════════════════════╗
║ WiFi DEVICE DETECTED
╠════════════════════════════════════════════════════════════╣
║ Name: Police_MDT_Unit5
║ Address: A4:B1:C1:23:45:67
║ Vendor: Cradlepoint
║ Type: EMERGENCY_VEHICLE
╠════════════════════════════════════════════════════════════╣
║ RSSI: -68 dBm
║ Signal Quality: GOOD
╠════════════════════════════════════════════════════════════╣
║ 📏 DISTANCE ANALYSIS:
║   Current: 18.7 m 🟡
║   Smoothed: 19.2 m [CLOSE]
║   Closest: 18.7 m
║   Farthest: 45.3 m
╠════════════════════════════════════════════════════════════╣
║ 🎯 MOVEMENT TRACKING:
║   Trend: ⬇️ APPROACHING
║   Velocity: 0.85 m/s (Walking)
║   Est. in 5s: 14.9 m
║   ⚠️  WARNING: Will be CLOSE in 5 seconds!
╠════════════════════════════════════════════════════════════╣
║ Status: KNOWN DEVICE
║ Detections: 15 | Updates: 12
║ First seen: 45s ago
╚════════════════════════════════════════════════════════════╝
!!! ALERT: EMERGENCY_VEHICLE @ 19.2 m !!!
```

### Statistical Summary

Every 30 seconds, comprehensive statistics are displayed:

```
╔════════════════════════════════════════════════════════════╗
║          📊 COMPREHENSIVE SUMMARY REPORT                   ║
╚════════════════════════════════════════════════════════════╝
║ Total tracked devices: 23
╠════════════════════════════════════════════════════════════╣
║ 📏 DISTANCE DISTRIBUTION:
║   🔴 Immediate (< 2m):    2
║   🟠 Very Close (2-10m):  5
║   🟡 Close (10-25m):      8
║   🟢 Nearby (25-50m):     6
║   🔵 Moderate (50-100m):  2
║   ⚪ Far (> 100m):        0
╠════════════════════════════════════════════════════════════╣
║ 🎯 MOVEMENT ANALYSIS:
║   ⬇️  Approaching:         7
║   ⬆️  Receding:            3
║   ↔️  Stable:              13
╠════════════════════════════════════════════════════════════╣
║ ⭐ CLOSEST DEVICE:
║   Distance: 1.8 m
║   Address: A4:B1:C1:23:45:67
╚════════════════════════════════════════════════════════════╝
```

## Accuracy Considerations

### Factors Affecting Accuracy

**Positive Factors (Improve Accuracy):**
- ✅ Line of sight (no obstacles)
- ✅ Open outdoor environment
- ✅ Device-specific calibration
- ✅ Multiple measurements (averaging)
- ✅ Known device type
- ✅ Stable RSSI signal

**Negative Factors (Reduce Accuracy):**
- ❌ Walls and obstacles
- ❌ Metal objects nearby
- ❌ Multipath reflections (indoor)
- ❌ WiFi interference
- ❌ Device transmit power variation
- ❌ Antenna orientation
- ❌ Weather conditions

### Typical Accuracy

| Environment | Accuracy | Notes |
|-------------|----------|-------|
| Open outdoor | ±2-5m | Best case scenario |
| Indoor line-of-sight | ±3-8m | Same room, no walls |
| Indoor through walls | ±5-15m | Path loss varies greatly |
| Dense urban | ±10-20m | Multipath, interference |

**General Rule:**
- **< 10m**: Accuracy within ±2-5m (good)
- **10-50m**: Accuracy within ±5-10m (moderate)
- **> 50m**: Accuracy within ±10-20m (rough estimate)

## Configuration

### Adjusting Scan Intervals

For more responsive tracking, decrease scan intervals:

```cpp
#define WIFI_SCAN_INTERVAL 3000    // 3 seconds (default)
#define BLE_SCAN_INTERVAL 3000     // 3 seconds (default)
```

For battery saving, increase intervals:
```cpp
#define WIFI_SCAN_INTERVAL 10000   // 10 seconds
#define BLE_SCAN_INTERVAL 10000    // 10 seconds
```

### Adjusting Alert Distances

```cpp
#define PROXIMITY_ALERT_DISTANCE 10.0    // Alert when < 10m
#define CLOSE_ALERT_DISTANCE 5.0         // Close alert < 5m
#define IMMEDIATE_ALERT_DISTANCE 2.0     // Immediate alert < 2m
```

### Adjusting Movement Sensitivity

In `distance_analysis.h`:

```cpp
// Change velocity threshold for movement detection
float velocityThreshold = 0.2;  // 0.2 m/s (default)
                                // Lower = more sensitive
                                // Higher = less sensitive
```

### Adjusting Smoothing

In `distance_analysis.h`:

```cpp
// Change smoothing alpha value
float alpha = 0.3;  // 0.3 = 30% new, 70% old (default)
                    // Higher alpha = more responsive, less smooth
                    // Lower alpha = smoother, less responsive
```

## Use Cases

### 1. Situational Awareness
Monitor surroundings and know when vehicles or people are approaching.

**Example Output:**
```
[LIVE] 🔴 ⬇️ Police Cruiser @ 1.5 m (-2.1 m/s)
⚠️  WARNING: EMERGENCY_VEHICLE approaching fast!
```

### 2. Parking Lot Navigation
Know when your car is nearby when returning to parking lot.

**Example Output:**
```
[LIVE] 🟡 ↔️ My_Car_Bluetooth @ 23.4 m (0.0 m/s)
Walking towards car...
[LIVE] 🟠 ⬇️ My_Car_Bluetooth @ 8.2 m (-0.6 m/s)
```

### 3. Security Monitoring
Track when cameras or monitoring devices are in range.

**Example Output:**
```
[LIVE] 🟢 ↔️ IP_Camera_North @ 35.6 m (0.0 m/s)
[LIVE] 🟡 ⬇️ IP_Camera_East @ 12.3 m (-0.3 m/s)
⚠️  CAMERA approaching!
```

### 4. Traffic Analysis
Monitor traffic flow and vehicle density.

**Example Output:**
```
MOVEMENT ANALYSIS:
  Approaching: 12 (vehicles coming towards you)
  Receding: 8 (vehicles moving away)
  Stable: 5 (parked or stationary)
```

## Advanced Features

### Distance Prediction

The system can predict where a device will be in the future:

```cpp
float predicted5s = predictDistance(address, 5);  // 5 seconds ahead
```

**Example:**
```
Current: 15.2 m
Velocity: -1.2 m/s (approaching)
Predicted in 5s: 9.2 m
Predicted in 10s: 3.2 m
```

### Proximity Detection

Check if device is within a specific range:

```cpp
bool nearby = isWithinProximity(address, 10.0);  // Within 10m?
```

### Signal Quality Assessment

Evaluates signal quality considering distance:

```
Distance: 25m, RSSI: -72 dBm → GOOD
Distance: 25m, RSSI: -85 dBm → POOR (obstacles likely)
```

## Troubleshooting

### Distances Seem Too Short
- Device may be transmitting at higher power than calibrated
- Adjust TX power in `distance_analysis.h`
- Add device-specific calibration

### Distances Seem Too Long
- Device may be transmitting at lower power (BLE)
- Check if using correct device type
- Verify path loss exponent

### Erratic Distance Readings
- Normal in environments with interference
- Increase smoothing (lower alpha value)
- Increase measurement history size

### Movement Not Detected
- Velocity threshold may be too high
- Lower threshold in `distance_analysis.h`
- Ensure scan intervals are fast enough

### Too Many False Alerts
- Increase distance thresholds
- Disable alerts for unwanted device types
- Adjust RSSI threshold to filter distant devices

## Technical Details

### Memory Usage

Per tracked device:
- Base info: ~200 bytes
- Distance history (10 readings): ~80 bytes
- RSSI history (10 readings): ~40 bytes
- **Total: ~320 bytes per device**

ESP32 has ~320KB RAM, supporting ~1000 devices theoretically.
Practical limit: 50-100 devices for stability.

### CPU Usage

- Distance calculation: ~0.1ms per device
- Smoothing: ~0.05ms per device
- Trend analysis: ~0.1ms per device
- **Total: ~0.25ms per device per update**

At 3-second scan intervals with 50 devices:
- ~12.5ms per scan cycle
- CPU load: < 0.5%

### Update Rate

- New scan: Every 3 seconds (default)
- Distance update: Every scan
- Live display: Every 5 seconds
- Summary: Every 30 seconds

Effective position update rate: ~0.3 Hz (3 times per second)

## Comparison: Basic vs Live Distance

| Feature | Basic Scanner | Live Distance Tracker |
|---------|---------------|----------------------|
| Distance Estimation | ✅ Basic | ✅ Advanced (device-calibrated) |
| Signal Smoothing | ❌ No | ✅ Yes (EMA filtering) |
| Movement Detection | ❌ No | ✅ Yes (approaching/receding) |
| Velocity Calculation | ❌ No | ✅ Yes |
| Distance Prediction | ❌ No | ✅ Yes |
| Live Updates | ❌ No | ✅ Every 5 seconds |
| Distance History | ❌ No | ✅ 10 measurements |
| Proximity Alerts | ✅ Basic | ✅ Distance-aware |
| Statistics | ✅ Basic | ✅ Comprehensive |
| Memory Usage | Low | Medium |
| CPU Usage | Low | Low-Medium |

## Next Steps

1. **Test in Different Environments**: Indoor, outdoor, urban
2. **Calibrate for Your Devices**: Measure actual distances and adjust TX power
3. **Fine-tune Alerts**: Adjust thresholds for your use case
4. **Add Logging**: Record distance data to SD card for analysis
5. **Visualize**: Add OLED display for real-time distance graph

---

**Ready to track distances?** Upload `vehicle_tracker_live_distance.ino` and watch devices move in real-time! 🎯
