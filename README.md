# ESP32 Wireless Device Scanner

A comprehensive WiFi and Bluetooth scanner for ESP32 that detects and identifies nearby wireless devices including vehicles, cameras, smartphones, and other IoT devices. Built for situational awareness and educational purposes.

## Features

- **Dual-Mode Scanning**: WiFi (2.4GHz) and Bluetooth/BLE scanning
- **Device Identification**: Automatically identifies device types based on:
  - MAC address vendor lookup (OUI database)
  - Device names and SSIDs
  - Bluetooth broadcast characteristics
- **Signal Analysis**: RSSI monitoring and distance estimation
- **Visual Alerts**: LED indicators for specific device types
- **Audio Alerts**: Optional buzzer support for important detections
- **Device Tracking**: Historical tracking with first seen/last seen timestamps
- **Comprehensive Logging**: Serial output with detailed device information

## Detected Device Types

- **Emergency Vehicles**: Police cars, ambulances, fire trucks (WiFi hotspots, MDTs)
- **Cameras**: IP cameras, surveillance systems (Hikvision, Dahua, Axis, etc.)
- **Body Cameras**: Police body-worn cameras (Axon, WatchGuard)
- **Dash Cameras**: Vehicle dashboard cameras (VIOFO, Garmin, BlackVue)
- **Traffic Monitors**: Traffic sensors and monitoring systems
- **Vehicles**: Car Bluetooth systems (CarPlay, Android Auto, infotainment)
- **Mobile Hotspots**: Portable WiFi hotspots and MDTs (Mobile Data Terminals)
- **Smartphones**: iPhone, Android devices
- **Computers**: Laptops, tablets, rugged computers (Toughbooks)
- **IoT Devices**: Smart home devices, sensors
- **Audio Devices**: Headphones, speakers, earbuds
- **Wearables**: Smartwatches, fitness trackers

## Hardware Requirements

### Required
- **ESP32 Development Board** (any variant):
  - ESP32-WROOM-32
  - ESP32-S3
  - ESP32-C3
  - ESP32-DevKit
- **USB Cable** (for programming and power)
- **Computer** (for uploading code)

### Optional
- **LED** (if your board doesn't have built-in LED)
  - Connect to GPIO 2 with 220Ω resistor
- **Buzzer** (for audio alerts)
  - Active buzzer recommended
  - Connect to GPIO 4 with transistor driver
- **OLED/LCD Display** (for visual interface - future enhancement)
- **SD Card Module** (for data logging - future enhancement)
- **Battery Pack** (for portable operation)
  - LiPo battery with charging module
  - Power bank (USB)

## Wiring Diagram

### Basic Setup (LED only)
```
ESP32          Component
━━━━━          ━━━━━━━━━
GPIO 2  ──────  LED Anode (+)
GND     ──────  LED Cathode (-) [via 220Ω resistor]
GPIO 15 ──────  Status LED Anode (+)
GND     ──────  Status LED Cathode (-) [via 220Ω resistor]
```

### With Buzzer
```
ESP32          Buzzer
━━━━━          ━━━━━━
GPIO 4  ──────  Signal (+)
GND     ──────  Ground (-)
```

Note: For passive buzzers, you may need a transistor driver circuit.

## Software Setup

### Option 1: Arduino IDE

1. **Install Arduino IDE**
   - Download from [arduino.cc](https://www.arduino.cc/en/software)

2. **Install ESP32 Board Support**
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools → Board → Boards Manager
   - Search for "ESP32" and install "esp32 by Espressif Systems"

3. **Install Required Libraries**
   - The ESP32 BLE library is included with the ESP32 board package
   - No additional libraries needed!

4. **Upload Code**
   - Open `vehicle_tracker_enhanced.ino` in Arduino IDE
   - Select your board: Tools → Board → ESP32 Arduino → ESP32 Dev Module
   - Select your port: Tools → Port → (your COM port)
   - Click Upload

### Option 2: PlatformIO

1. **Install PlatformIO**
   - Install [VS Code](https://code.visualstudio.com/)
   - Install PlatformIO extension from VS Code marketplace

2. **Open Project**
   - Open the project folder in VS Code
   - PlatformIO will automatically detect `platformio.ini`

3. **Build and Upload**
   ```bash
   pio run --target upload
   ```

## Configuration

Edit the settings at the top of the `.ino` file or in `config.h`:

### Scan Settings
```cpp
#define WIFI_SCAN_INTERVAL 5000    // WiFi scan interval (ms)
#define BLE_SCAN_INTERVAL 5000     // BLE scan interval (ms)
#define BLE_SCAN_DURATION 3        // BLE scan duration (seconds)
#define RSSI_THRESHOLD -90         // Min signal strength (-90 to -30 dBm)
```

### Alert Settings
```cpp
#define ALERT_ON_EMERGENCY_VEHICLE true
#define ALERT_ON_CAMERA true
#define ALERT_ON_BODYCAM true
#define ALERT_ON_TRAFFIC_MONITOR true
```

### Pin Configuration
```cpp
#define ALERT_LED_PIN 2        // Alert LED pin
#define BUZZER_PIN 4           // Buzzer pin (-1 to disable)
#define STATUS_LED_PIN 15      // Status LED pin
```

## Usage

### Basic Operation

1. **Power On**
   - Connect ESP32 via USB or battery
   - System initializes in ~3 seconds

2. **Monitor Serial Output**
   - Open Serial Monitor (115200 baud)
   - Devices will be displayed as detected

3. **Interpret Results**
   - NEW devices trigger immediate alerts
   - Known devices update signal strength
   - Summary printed every 30 seconds

### Serial Output Example

```
╔════════════════════════════════════════════╗
║   ESP32 Wireless Device Scanner v2.0      ║
║   WiFi + BLE Scanner with Alerts          ║
╚════════════════════════════════════════════╝

Initializing...
✓ WiFi initialized
✓ BLE initialized

🔍 Scanning for devices...

[WiFi DEVICE DETECTED]
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Name: Squad_Car_23
Address: A4:B1:C1:23:45:67
Vendor: Cradlepoint
Type: EMERGENCY_VEHICLE
RSSI: -62 dBm (STRONG)
Est. Distance: ~15.3 meters
Status: NEW DEVICE
Detections: 1
First seen: 0 seconds ago
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
!!! ALERT TRIGGERED: EMERGENCY_VEHICLE - Squad_Car_23 !!!
```

### Understanding Signal Strength

| RSSI (dBm) | Strength | Approx. Distance |
|------------|----------|------------------|
| -30 to -50 | Very Strong | 0-10 meters |
| -50 to -60 | Strong | 10-30 meters |
| -60 to -70 | Moderate | 30-50 meters |
| -70 to -80 | Weak | 50-100 meters |
| -80 to -90 | Very Weak | 100+ meters |

**Note**: Distance estimation is approximate and varies based on:
- Transmit power of device
- Obstacles (walls, vehicles, etc.)
- Interference from other devices
- Antenna orientation

## Customization

### Adding Custom Device Patterns

Edit `device_patterns.h` to add new detection patterns:

```cpp
// Add to namePatterns array
{"custom_keyword", "CUSTOM_TYPE", false},

// Add to vendorDatabase array
{"AA:BB:CC", "Custom Vendor", "CUSTOM_TYPE"},
```

### Changing Alert Behavior

```cpp
bool shouldAlert(String deviceType) {
  if (deviceType == "YOUR_TYPE") return true;
  // Add custom logic
  return false;
}
```

## Troubleshooting

### No Devices Detected
- Check antenna orientation
- Reduce `RSSI_THRESHOLD` (e.g., -95)
- Ensure WiFi/BLE modules are enabled
- Try different location (less interference)

### Compilation Errors
- Ensure ESP32 board package is installed
- Check board selection matches your hardware
- Update to latest ESP32 board package

### Upload Failed
- Check USB cable (use data cable, not charge-only)
- Select correct COM port
- Press BOOT button during upload (some boards)
- Try different USB port

### High False Positive Rate
- Increase `RSSI_THRESHOLD` (e.g., -70)
- Adjust detection patterns in `device_patterns.h`
- Disable unwanted device type alerts

### Memory Issues
- ESP32 has limited RAM
- Reduce scan intervals if tracking many devices
- Clear old devices periodically (add timeout logic)

## Performance Tips

1. **Battery Life**
   - Increase scan intervals (10-30 seconds)
   - Disable unused features (BLE or WiFi only)
   - Use deep sleep between scans

2. **Detection Range**
   - Use external antenna for better range
   - Position device away from metal objects
   - Elevate scanner for line-of-sight

3. **Accuracy**
   - Calibrate distance estimation for your environment
   - Maintain MAC vendor database
   - Test in known scenarios

## Legal and Ethical Considerations

⚠️ **IMPORTANT DISCLAIMER**

This tool is designed for **educational purposes** and **legitimate awareness** of wireless devices in your environment.

### Legal Usage
✅ **Acceptable Uses:**
- Learning about wireless protocols and ESP32 development
- Personal awareness of devices in public spaces
- Security research with proper authorization
- Network diagnostics on your own networks
- Educational demonstrations

❌ **Prohibited Uses:**
- Evading law enforcement
- Unauthorized surveillance or tracking
- Interfering with public safety systems
- Violating privacy laws
- Any malicious or illegal activities

### Privacy & Laws
- Passive scanning (monitoring broadcasts) is generally legal
- Device identification based on public broadcasts is legal
- **Check local laws** regarding wireless monitoring in your jurisdiction
- Do not use to track individuals without consent
- Respect privacy and use responsibly

### Responsible Use
- This tool only passively receives broadcast signals
- It does not connect to, hack, or interfere with devices
- Similar to WiFi analyzer apps available on smartphones
- Use for awareness, not evasion or harassment

**By using this code, you agree to use it legally and ethically.**

## Technical Details

### Scanning Methods

**WiFi Scanning:**
- Uses ESP32 WiFi radio in station mode
- Scans 2.4GHz channels 1-13
- Detects Access Points and some client devices
- Passive scanning (no association)

**Bluetooth Scanning:**
- BLE (Bluetooth Low Energy) advertisement scanning
- Detects beacons, peripherals, and connectable devices
- Active scanning requests scan response packets
- Classic Bluetooth not scanned (BLE only)

### Device Identification Logic

1. **MAC Address Lookup**: First 3 bytes (OUI) checked against vendor database
2. **Name Pattern Matching**: Device name/SSID checked for keywords
3. **Signal Characteristics**: RSSI patterns and broadcast intervals
4. **Combined Analysis**: Multiple factors weighted for classification

### Limitations

- **2.4GHz Only**: Does not scan 5GHz WiFi networks
- **BLE Only**: Classic Bluetooth (BR/EDR) not scanned
- **Passive Only**: Cannot decrypt encrypted communications
- **Range Limited**: Typical range 10-100 meters depending on environment
- **Distance Estimation**: Approximate only, affected by many factors
- **Hidden SSIDs**: WiFi networks with hidden SSID show as blank
- **Randomized MACs**: Some devices use random MAC addresses

## Future Enhancements

Potential additions (contributions welcome):
- [ ] OLED/LCD display support
- [ ] SD card logging
- [ ] GPS integration for location tracking
- [ ] Web interface for remote monitoring
- [ ] 5GHz WiFi scanning (on compatible boards)
- [ ] Classic Bluetooth scanning
- [ ] Machine learning for device classification
- [ ] Mobile app companion
- [ ] Multi-device mesh networking
- [ ] Historical analysis and patterns

## Contributing

Contributions are welcome! Areas for improvement:
- Additional device patterns and MAC vendor database
- Better distance estimation algorithms
- Power optimization
- Display interface implementations
- Documentation improvements

## License

This project is provided as-is for educational purposes. Use responsibly and legally.

## Acknowledgments

- Built with ESP32 Arduino Core
- Uses Espressif BLE libraries
- MAC vendor database from IEEE OUI registry

## Support

For issues, questions, or contributions:
- Check the troubleshooting section
- Review ESP32 Arduino documentation
- Ensure hardware connections are correct
- Test with known devices first

## Version History

- **v2.0** - Enhanced version with alerts, vendor lookup, device tracking
- **v1.0** - Basic WiFi and BLE scanning

---

**Remember**: Use this tool responsibly and legally. It's designed for awareness and education, not for evading law enforcement or violating privacy.
