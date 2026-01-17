# Quick Start Guide

Get your ESP32 Vehicle Tracker running in 10 minutes!

## What You Need

✅ ESP32 board
✅ USB cable
✅ Computer with Arduino IDE

That's it! LEDs and buzzer are optional.

## 5-Minute Setup

### 1. Install Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software)

### 2. Add ESP32 Support
- Open Arduino IDE
- File → Preferences
- Add this URL to "Additional Board Manager URLs":
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- Tools → Board → Boards Manager
- Search "ESP32" and install

### 3. Upload Code
- Connect ESP32 via USB
- Open `vehicle_tracker_enhanced.ino`
- Tools → Board → ESP32 Dev Module
- Tools → Port → (select your port)
- Click Upload ⬆️

### 4. Watch the Magic
- Tools → Serial Monitor
- Set baud rate to **115200**
- Watch devices appear!

## What You'll See

```
╔════════════════════════════════════════════╗
║   ESP32 Wireless Device Scanner v2.0      ║
╚════════════════════════════════════════════╝

[WiFi DEVICE DETECTED]
Name: NETGEAR-5G
Type: ROUTER
RSSI: -65 dBm (MODERATE)
Est. Distance: ~25 meters

[BLE DEVICE DETECTED]
Name: John's iPhone
Type: SMARTPHONE
RSSI: -45 dBm (VERY_STRONG)
Est. Distance: ~5 meters
```

## Customization

Want to change what triggers alerts? Edit these lines in the code:

```cpp
#define ALERT_ON_EMERGENCY_VEHICLE true  // Police cars, ambulances
#define ALERT_ON_CAMERA true             // Cameras
#define ALERT_ON_BODYCAM true            // Body cameras
#define ALERT_ON_TRAFFIC_MONITOR true    // Traffic sensors
```

Set to `false` to disable specific alerts.

## Troubleshooting

**No devices showing up?**
- Move to area with more WiFi/Bluetooth devices
- Lower the RSSI threshold: `#define RSSI_THRESHOLD -95`

**Can't upload code?**
- Check USB cable (must support data)
- Select correct COM port
- Press BOOT button during upload

**Compilation error?**
- Make sure ESP32 board package is installed
- Restart Arduino IDE

## Next Steps

- Read [README.md](README.md) for full documentation
- Check [WIRING.md](WIRING.md) to add LEDs and buzzer
- Explore [examples/basic_scanner.ino](examples/basic_scanner.ino) for simpler version

## Using It

### Portable Mode
1. Disconnect from computer
2. Power with USB battery pack
3. Built-in LED will blink when devices detected
4. Connect to laptop occasionally to view logs

### Desktop Mode
1. Keep connected to computer
2. Serial Monitor shows real-time detections
3. Best for learning and testing

## Tips

💡 **Best range**: Elevate the ESP32 for line-of-sight
💡 **Battery life**: Use power bank for 4-8 hours runtime
💡 **Stealthy**: Use minimal version without buzzer
💡 **Learning**: Start with basic_scanner.ino first

## Understanding Results

### Signal Strength
- **VERY_STRONG** (-30 to -50 dBm): Device is very close (< 10m)
- **STRONG** (-50 to -60 dBm): Device nearby (10-30m)
- **MODERATE** (-60 to -70 dBm): Medium distance (30-50m)
- **WEAK** (-70 to -80 dBm): Far away (50-100m)
- **VERY_WEAK** (-80 to -90 dBm): Very far (100m+)

### Device Types
Scanner can identify:
- 🚓 Emergency vehicles (police, fire, ambulance)
- 📷 Cameras (surveillance, IP cameras)
- 🚗 Vehicle systems (CarPlay, Bluetooth)
- 📱 Smartphones and tablets
- 💻 Computers and laptops
- 🎧 Headphones and speakers
- And many more!

## Safety & Legal

⚠️ **Use Responsibly**
- Educational purposes only
- Passive scanning is generally legal
- Do NOT use to evade law enforcement
- Respect privacy laws
- Check local regulations

This tool only listens to public wireless broadcasts - just like a WiFi analyzer app on your phone.

## Support

Having issues?
1. Check the troubleshooting section above
2. Read [README.md](README.md) for detailed docs
3. Verify your wiring with [WIRING.md](WIRING.md)
4. Make sure you're using a real ESP32 board

## Code Versions

Choose the right version for you:

| File | Best For | Features |
|------|----------|----------|
| `vehicle_tracker_enhanced.ino` | Most users | Full features, alerts, tracking |
| `vehicle_tracker.ino` | Simple use | Basic scanning, no alerts |
| `examples/basic_scanner.ino` | Learning | Minimal code, easy to understand |

Start with enhanced version - it's the most complete!

---

**You're all set!** 🎉

The scanner is now running. Walk around and watch it detect devices. Have fun learning about the wireless world around you!
