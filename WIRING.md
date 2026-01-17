# Wiring Guide for ESP32 Vehicle Tracker

## Components Needed

### Required
- ESP32 Development Board (any variant)
- USB cable (for programming and power)

### Optional
- 2x LEDs (5mm, any color)
- 2x 220Ω resistors (for LEDs)
- Active buzzer (3-5V)
- Breadboard
- Jumper wires
- Battery pack (for portable use)

## Pin Assignments

| Component | ESP32 Pin | Notes |
|-----------|-----------|-------|
| Alert LED | GPIO 2 | Built-in LED on most boards |
| Status LED | GPIO 15 | External LED recommended |
| Buzzer | GPIO 4 | Optional, can disable in code |
| Power | VIN/5V | 5V from USB or battery |
| Ground | GND | Common ground |

## Wiring Diagrams

### Minimal Setup (Built-in LED only)

```
┌─────────────────┐
│     ESP32       │
│                 │
│  [Built-in LED] │ ← GPIO 2 (Alert)
│                 │
│      [USB]      │ ← Power & Programming
└─────────────────┘
```

No external wiring needed! The basic code works with just USB power.

### Basic Setup (With External LEDs)

```
                    ESP32
                 ┌─────────┐
                 │         │
    Alert LED    │  GPIO 2 ├──────┬───[220Ω]───[LED+]───[LED-]───GND
                 │         │
   Status LED    │ GPIO 15 ├──────┬───[220Ω]───[LED+]───[LED-]───GND
                 │         │
                 │   GND   ├──────┴─────────────────────────────┘
                 │         │
                 │   VIN   ├────── 5V Power
                 └─────────┘
```

### Full Setup (With Buzzer)

```
                    ESP32
                 ┌─────────┐
                 │         │
    Alert LED    │  GPIO 2 ├────── [220Ω] ─── [LED] ─── GND
                 │         │
   Status LED    │ GPIO 15 ├────── [220Ω] ─── [LED] ─── GND
                 │         │
    Buzzer       │  GPIO 4 ├────── [BUZZER+]
                 │         │         [BUZZER-]
                 │   GND   ├──────────────┴────────────────┘
                 │         │
                 │   VIN   ├────── 5V Power
                 └─────────┘
```

## Breadboard Layout

```
        ESP32 Board
     ┌──────────────┐
     │              │
     │     [USB]    │
     │              │
     └──┬────┬────┬─┘
        │    │    │
     ───┼────┼────┼───  Breadboard
        │    │    │
      GPIO2 GPIO15 GPIO4
        │    │    │
        │    │    └──── Buzzer (+)
        │    │
        │    └────────── Status LED → 220Ω → GND
        │
        └─────────────── Alert LED → 220Ω → GND


      All grounds connect to ESP32 GND pin
```

## Step-by-Step Assembly

### Step 1: ESP32 on Breadboard
1. Insert ESP32 into breadboard (straddle the center gap)
2. Ensure pins are properly seated
3. Leave room on both sides for connections

### Step 2: Alert LED (GPIO 2)
1. Insert LED into breadboard (long leg = anode/+)
2. Connect GPIO 2 to LED anode (long leg)
3. Connect 220Ω resistor from LED cathode (short leg) to ground rail
4. **Note**: Many ESP32 boards have built-in LED on GPIO 2

### Step 3: Status LED (GPIO 15)
1. Insert second LED into breadboard
2. Connect GPIO 15 to LED anode
3. Connect 220Ω resistor from LED cathode to ground rail

### Step 4: Buzzer (GPIO 4) - Optional
1. Insert buzzer into breadboard (or connect with wires)
2. Connect buzzer **+** pin to GPIO 4
3. Connect buzzer **-** pin to ground rail
4. **Important**: Verify buzzer polarity!

### Step 5: Power Connections
1. Connect ESP32 GND pin to breadboard ground rail
2. Connect all component grounds to same ground rail
3. Power ESP32 via USB or VIN pin (5V)

## LED Behavior

| LED | State | Meaning |
|-----|-------|---------|
| Alert LED | OFF | Normal operation, no alerts |
| Alert LED | ON (solid) | Alert active (important device detected) |
| Alert LED | Flashing | Initialization/test |
| Status LED | OFF | Idle |
| Status LED | ON | Scanning in progress |

## Buzzer Patterns

| Pattern | Meaning |
|---------|---------|
| 3 quick beeps | Emergency vehicle or body camera detected |
| 1 long beep | Camera or traffic monitor detected |
| No sound | Buzzer disabled or no alerts |

## Power Options

### USB Power (Easiest)
```
Computer USB ──[USB Cable]──> ESP32
```
- Simplest option
- Good for testing and development
- ~5V @ 500mA available

### Battery Pack (Portable)
```
5V Battery Pack ──> ESP32 VIN pin
                └──> ESP32 GND pin
```
- USB power bank (5V)
- LiPo battery with 5V regulator
- Typical runtime: 2-8 hours depending on battery capacity

### LiPo Battery (Advanced)
```
3.7V LiPo ──[Charge Module]──> ESP32 VIN
                             └> ESP32 GND
```
- Rechargeable option
- Requires battery management board
- Longer runtime with 2000+ mAh battery

## Enclosure Ideas

### Quick & Easy
- Cardboard box with holes for LEDs
- 3D printed case
- Plastic project box from electronics store

### Professional
- Custom 3D printed enclosure
- Waterproof case for outdoor use
- With battery, switch, and mounting holes

## Safety Notes

⚠️ **Important Safety Information**

1. **Polarity**: Double-check LED and buzzer polarity before powering on
2. **Resistors**: Always use resistors with LEDs (220Ω recommended)
3. **Power**: Don't exceed 5V on VIN pin or 3.3V on GPIO pins
4. **Current**: Total current draw should not exceed 500mA from USB
5. **Heat**: ESP32 may get warm during operation (normal)
6. **Shorts**: Ensure no wires are touching/shorted
7. **Battery**: Use protected LiPo batteries with proper charging circuits

## Troubleshooting Wiring

### LED Not Lighting
- Check polarity (long leg = +)
- Verify resistor value (220Ω)
- Test LED with multimeter
- Ensure GPIO pin is correct

### Buzzer Not Working
- Verify it's an active buzzer (has internal oscillator)
- Check polarity
- Test with multimeter (should show ~50Ω resistance)
- Try different GPIO pin in code

### ESP32 Not Powering On
- Check USB cable (must support data)
- Verify power source
- Look for short circuits
- Try different USB port

### Erratic Behavior
- Check for loose connections
- Verify ground connections
- Ensure power supply is adequate
- Look for interference from buzzer

## Testing Your Build

1. **Power Test**
   - Plug in ESP32
   - Built-in power LED should light up
   - No smoke or unusual smells

2. **LED Test**
   - Upload basic blink sketch
   - Verify both LEDs work
   - Check brightness

3. **Buzzer Test**
   - Upload tone sketch
   - Listen for sound
   - Should be clear, not distorted

4. **Full System Test**
   - Upload scanner code
   - Open serial monitor
   - Watch for scan activity
   - Verify alerts trigger correctly

## Advanced Modifications

### Add OLED Display
- Connect I2C OLED (0.96" or 1.3")
- SDA → GPIO 21
- SCL → GPIO 22
- VCC → 3.3V
- GND → GND

### Add SD Card Logger
- Connect SPI SD card module
- CS → GPIO 5
- SCK → GPIO 18
- MISO → GPIO 19
- MOSI → GPIO 23

### External Antenna
- Some ESP32 boards support external antenna
- Improves range significantly
- Check board documentation

## Bill of Materials (BOM)

| Component | Quantity | Approx. Cost (USD) |
|-----------|----------|-------------------|
| ESP32 Dev Board | 1 | $5-10 |
| LED (5mm) | 2 | $0.10 |
| 220Ω Resistor | 2 | $0.05 |
| Active Buzzer | 1 | $0.50 |
| Breadboard | 1 | $2-5 |
| Jumper Wires | 10 | $1 |
| USB Cable | 1 | $1-3 |
| **Total** | | **~$10-20** |

Optional:
- Battery pack: $5-15
- Project enclosure: $3-10
- OLED display: $3-8

## Where to Buy

- **AliExpress**: Cheapest, slow shipping
- **Amazon**: Fast shipping, higher price
- **Adafruit/SparkFun**: Quality components, educational
- **Local electronics store**: Immediate availability

## Next Steps

1. ✅ Build circuit on breadboard
2. ✅ Test with basic blink sketch
3. ✅ Upload scanner code
4. ✅ Monitor serial output
5. ✅ Test in different environments
6. 🔧 Build permanent version (optional)
7. 📦 Design enclosure (optional)

Happy building! 🔧
