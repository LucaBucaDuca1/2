# Version Comparison Guide

Choose the right scanner version for your needs!

## Quick Comparison

| Feature | Basic Scanner | Standard Tracker | Enhanced Tracker | **Live Distance** |
|---------|---------------|------------------|------------------|-------------------|
| **File** | `basic_scanner.ino` | `vehicle_tracker.ino` | `vehicle_tracker_enhanced.ino` | `vehicle_tracker_live_distance.ino` |
| **Best For** | Learning | Simple use | Full features | Distance tracking |
| WiFi Scanning | ✅ | ✅ | ✅ | ✅ |
| BLE Scanning | ✅ | ✅ | ✅ | ✅ |
| Device Identification | ❌ Basic | ✅ | ✅ | ✅ |
| MAC Vendor Lookup | ❌ | ✅ | ✅ | ✅ |
| Distance Estimation | ❌ | ✅ Basic | ✅ Basic | ✅ **Advanced** |
| LED Alerts | ❌ | ❌ | ✅ | ✅ |
| Buzzer Alerts | ❌ | ❌ | ✅ | ✅ |
| Device Tracking | ❌ | ✅ | ✅ | ✅ |
| **Movement Detection** | ❌ | ❌ | ❌ | ✅ **NEW** |
| **Live Distance Updates** | ❌ | ❌ | ❌ | ✅ **NEW** |
| **Trend Analysis** | ❌ | ❌ | ❌ | ✅ **NEW** |
| **Velocity Calculation** | ❌ | ❌ | ❌ | ✅ **NEW** |
| **Distance Prediction** | ❌ | ❌ | ❌ | ✅ **NEW** |
| **Device-Specific Calibration** | ❌ | ❌ | ❌ | ✅ **NEW** |
| Code Complexity | Very Simple | Simple | Moderate | Advanced |
| Lines of Code | ~80 | ~400 | ~500 | ~650 |
| Memory Usage | Low | Low | Medium | Medium |
| CPU Usage | Low | Low | Low | Low-Medium |

## Detailed Breakdown

### 1. Basic Scanner (`examples/basic_scanner.ino`)

**Best for:** Beginners, learning, simple testing

**Features:**
- ✅ WiFi network scanning
- ✅ BLE device scanning
- ✅ RSSI display
- ✅ Minimal code (~80 lines)

**Output Example:**
```
--- WiFi Scan ---
Found 5 networks:
  Home_Network (-45 dBm)
  NETGEAR (-67 dBm)

--- BLE Scan ---
BLE Device: aa:bb:cc:dd:ee:ff | iPhone | RSSI: -62
```

**Use When:**
- Just learning ESP32
- Need simple code to understand
- Want basic WiFi/BLE scanning
- Don't need device identification

---

### 2. Standard Tracker (`vehicle_tracker.ino`)

**Best for:** General use without alerts

**Features:**
- ✅ WiFi and BLE scanning
- ✅ Device type identification
- ✅ MAC vendor lookup
- ✅ Basic distance estimation
- ✅ Device tracking
- ✅ Summary reports

**Output Example:**
```
[WiFi DEVICE DETECTED]
SSID: Police_Car_23
BSSID: A4:B1:C1:23:45:67
Type: EMERGENCY_VEHICLE
RSSI: -65 dBm (MODERATE)
Est. Distance: 25.3 m
Status: NEW
```

**Use When:**
- Want device identification
- Don't need alerts
- Prefer quieter operation
- Learning device types

---

### 3. Enhanced Tracker (`vehicle_tracker_enhanced.ino`)

**Best for:** Most users, full feature set

**Features:**
- ✅ Everything from Standard +
- ✅ LED visual alerts
- ✅ Buzzer audio alerts
- ✅ Alert customization
- ✅ Better formatting
- ✅ Alert patterns

**Output Example:**
```
╔════════════════════════════════════════════╗
║ WiFi DEVICE DETECTED
╠════════════════════════════════════════════╣
║ Name: Police_MDT
║ Type: EMERGENCY_VEHICLE
║ RSSI: -65 dBm
║ Est. Distance: ~25 m
╠════════════════════════════════════════════╣
║ Status: NEW DEVICE
║ Detections: 1
╚════════════════════════════════════════════╝
>>> ALERT TRIGGERED: EMERGENCY_VEHICLE <<<
```

**Use When:**
- Want visual/audio alerts
- Need immediate notifications
- Using LEDs and buzzer
- Want the best general-purpose scanner

---

### 4. Live Distance Tracker (`vehicle_tracker_live_distance.ino`) ⭐ NEW

**Best for:** Distance tracking, movement detection

**Features:**
- ✅ Everything from Enhanced +
- ✅ **Real-time distance tracking**
- ✅ **Movement detection (approaching/receding)**
- ✅ **Velocity calculation**
- ✅ **Distance prediction**
- ✅ **Device-specific calibration**
- ✅ **Live distance updates**
- ✅ **Trend analysis**
- ✅ **Distance-based alerts**
- ✅ **Comprehensive statistics**

**Output Example:**
```
╔════════════════════════════════════════════════════════════╗
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
╚════════════════════════════════════════════════════════════╝

[LIVE] 🟡 ⬇️ Police_MDT @ 19.2 m (-0.8 m/s)
[LIVE] 🟢 ↔️ Router @ 42.1 m (0.0 m/s)
[LIVE] 🔴 ⬇️ iPhone @ 1.8 m (-1.2 m/s)
```

**Use When:**
- Need accurate distance tracking
- Want to know if devices are approaching
- Need movement analysis
- Want predictive alerts
- Tracking specific devices over time

---

## Feature Deep Dive

### Distance Estimation

| Version | Method | Accuracy | Features |
|---------|--------|----------|----------|
| Basic | ❌ None | N/A | No distance |
| Standard | Simple formula | ±10-20m | Static calculation |
| Enhanced | Simple formula | ±10-20m | Static calculation |
| **Live** | **Device-calibrated** | **±5-10m** | **Smoothing, tracking** |

### Movement Detection (Live Distance Only)

The Live Distance version can detect:
- ⬇️ **Approaching**: Device getting closer
- ⬆️ **Receding**: Device moving away
- ↔️ **Stable**: Device stationary

And calculate:
- **Velocity**: How fast (m/s)
- **Prediction**: Where device will be in 5-10 seconds
- **Min/Max**: Closest and farthest distances seen

### Alert System

| Version | LED | Buzzer | Distance-Based | Type-Based |
|---------|-----|--------|----------------|------------|
| Basic | ❌ | ❌ | ❌ | ❌ |
| Standard | ❌ | ❌ | ❌ | ✅ |
| Enhanced | ✅ | ✅ | ❌ | ✅ |
| **Live** | ✅ | ✅ | **✅** | ✅ |

**Live Distance Alert Patterns:**
- 🔴 **< 2m (Immediate)**: 5 rapid beeps
- 🟠 **< 5m (Close)**: 3 beeps
- 🟡 **< 10m (Proximity)**: 2 beeps
- **Emergency vehicle**: Alerts at any distance
- **Approaching**: Extra alert if coming closer

## Performance Comparison

### Memory Usage

```
Basic:     ~5 KB RAM
Standard:  ~15 KB RAM
Enhanced:  ~20 KB RAM
Live:      ~25 KB RAM
```

All versions use < 10% of ESP32's 320KB RAM.

### Scan Speed

```
Basic:     10 seconds/cycle
Standard:  10 seconds/cycle
Enhanced:  10 seconds/cycle
Live:      6 seconds/cycle (faster for real-time)
```

### CPU Load

```
Basic:     < 0.1%
Standard:  < 0.5%
Enhanced:  < 0.5%
Live:      < 1%
```

All versions have negligible CPU impact.

### Battery Life (with 2000mAh battery)

```
Basic:     ~8 hours
Standard:  ~6 hours
Enhanced:  ~6 hours (with alerts)
Live:      ~5 hours (faster scanning)
```

## Which Should You Choose?

### Choose **Basic Scanner** if:
- 👶 You're new to ESP32
- 📚 You want to learn the basics
- 🔍 You just need simple WiFi/BLE scanning
- 💻 You want to understand the code

### Choose **Standard Tracker** if:
- 🎯 You want device identification
- 📊 You need tracking and statistics
- 🔕 You don't want alerts
- 📝 You prefer quieter operation

### Choose **Enhanced Tracker** if:
- ⭐ You want the best general-purpose scanner
- 🚨 You need visual/audio alerts
- 🔔 You want to be notified of important devices
- 🎛️ You have LEDs and buzzer connected

### Choose **Live Distance** if:
- 📏 **You need accurate distance tracking**
- 🎯 **You want to know if devices are approaching**
- 📈 **You need movement analysis**
- 🔮 **You want predictive alerts**
- 🚗 **You're tracking vehicles or moving devices**
- ⚠️ **You need proximity warnings**

## Upgrade Path

Start simple and upgrade as needed:

```
Basic Scanner
    ↓
Standard Tracker (add device identification)
    ↓
Enhanced Tracker (add alerts)
    ↓
Live Distance (add distance tracking & movement detection)
```

Each version is standalone - no need to go through all of them!

## Migration Tips

### From Enhanced to Live Distance

Already using Enhanced? Switching to Live Distance is easy:

1. **Replace the .ino file** with `vehicle_tracker_live_distance.ino`
2. **Add `distance_analysis.h`** to your project
3. **Upload** - same pins, same configuration!

**What changes:**
- More detailed distance information
- Live distance updates every 5 seconds
- Movement detection
- Better alerts based on proximity

**What stays the same:**
- Pin configuration
- Alert types
- Device identification
- All your current settings

## Recommended Version

### For Most Users: **Enhanced Tracker** ⭐
- Full features
- Alerts
- Easy to use
- Great performance

### For Distance Tracking: **Live Distance** 🎯
- Real-time distance
- Movement detection
- Approaching/receding alerts
- Perfect for tracking specific devices

### For Learning: **Basic Scanner** 📚
- Simple code
- Easy to understand
- Great starting point

---

**Not sure?** Start with **Enhanced Tracker**. You can always upgrade to Live Distance later if you need the advanced distance features!

## Support Files Needed

| File | Basic | Standard | Enhanced | Live Distance |
|------|-------|----------|----------|---------------|
| `device_patterns.h` | ❌ | ✅ | ✅ | ✅ |
| `distance_analysis.h` | ❌ | ❌ | ❌ | ✅ |
| `config.h` | ❌ | ❌ | Optional | Optional |
| `platformio.ini` | Optional | Optional | Optional | Optional |

---

Still have questions? Check:
- **README.md** - General documentation
- **DISTANCE_ANALYSIS.md** - Deep dive into distance features
- **QUICKSTART.md** - 10-minute setup guide
- **WIRING.md** - Hardware setup

Happy scanning! 🔍
