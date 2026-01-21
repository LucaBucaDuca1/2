# HackRF PortaPack RF Device Scanner

## Overview

The **RF Device Scanner** is a custom application for the HackRF PortaPack that detects and identifies radio frequency signals from government, law enforcement, emergency services, and public safety devices in real-time.

This application provides passive RF monitoring with intelligent signal identification, focusing on frequencies used by agencies in Los Angeles County and Southern California.

## Features

### Core Capabilities

- **Wide-Band RF Scanning**: Scan from 150 MHz to 900 MHz
- **Intelligent Signal Detection**: Identify devices based on comprehensive frequency database
- **Real-Time Analysis**: Live RSSI monitoring and signal strength display
- **Multi-Band Support**: Quick-select common public safety bands
- **Detection Logging**: Save scan results with timestamps
- **Signal History Tracking**: Track signal patterns over time
- **Confidence Scoring**: Rate detection accuracy based on signal strength and frequency match

### Scan Bands

1. **VHF Low Band (150-174 MHz)**: Police, Fire, Business radios
2. **VHF High Band (174-216 MHz)**: Government, Military
3. **UHF-T Band (450-470 MHz)**: Public Safety, Business
4. **UHF Public Safety (470-512 MHz)**: Government exclusive
5. **700 MHz Band (700-800 MHz)**: FirstNet LTE Public Safety
6. **800 MHz Band (806-869 MHz)**: Trunked Radio Systems (P25, Motorola)
7. **900 MHz Band (896-960 MHz)**: Cellular, Additional trunked systems
8. **Full Scan Mode**: All bands (150-900 MHz)

## Frequency Database

The app includes **300+ known frequencies** for:

### Law Enforcement (80+ frequencies)
- **LAPD** (Los Angeles Police Department): Dispatch, Tactical, SWAT, Air Support
- **LASD** (LA County Sheriff): Patrol divisions, Special units
- **CHP** (California Highway Patrol): Regional channels, Air ops
- **Municipal Police**: Beverly Hills, Santa Monica, Long Beach, Pasadena, Glendale, Burbank, Torrance
- **Federal Agencies**: FBI, DEA, ATF, Secret Service, US Marshals, Border Patrol, ICE, DHS

### Fire & EMS (30+ frequencies)
- **LAFD** (Los Angeles Fire Department): Dispatch, Tactical, Command, Arson
- **LACoFD** (LA County Fire): Regional channels
- **CalFire**: Wildfire operations, Air support
- **EMS**: Hospital links, Ambulance dispatch, Paramedic channels

### Government & Transportation (25+ frequencies)
- **LA Metro**: Rail, Bus, Security
- **Caltrans**: Traffic management, Maintenance
- **Airport Security**: LAX, Burbank, Long Beach
- **Schools**: LAUSD Police, UCLA PD, USC Public Safety

### Specialized Systems (50+ frequencies)
- **Trunked Radio**: LA-RICS, 800 MHz control channels
- **FirstNet**: LTE Band 14 (Public Safety Broadband)
- **Traffic Systems**: ALPR (License Plate Readers), Speed sensors, Traffic cameras
- **Surveillance**: Body cameras, Dash cameras, Mobile surveillance

## Hardware Requirements

### Compatible Devices
- HackRF One + PortaPack H1/H2
- HackRF One + PortaPack H2M4
- HackRF One + PortaPack H4M

### Firmware
- **PortaPack Mayhem** (recommended, latest version)
- **PortaPack Havoc** (may require modifications)

### Antenna Requirements
- **Wide-band antenna**: 100 MHz - 1 GHz coverage
- **Telescopic antenna**: Included with most PortaPacks (acceptable)
- **Optimized antennas**: VHF/UHF dual-band for best performance
- **Range**: Typically 100m - 1km depending on signal strength and antenna

## Installation

### Method 1: Add to Existing Firmware Build

1. Clone PortaPack Mayhem firmware:
```bash
git clone https://github.com/eried/portapack-mayhem.git
cd portapack-mayhem
```

2. Copy application files:
```bash
cp rf_scanner.hpp firmware/application/
cp rf_scanner.cpp firmware/application/
cp freq_database.hpp firmware/application/
```

3. Register the app in `firmware/application/CMakeLists.txt`:
```cmake
# Add to CPPSRC list
rf_scanner.cpp

# Add to application list
add_application(rf_scanner rf_scanner.cpp)
```

4. Build firmware:
```bash
mkdir build && cd build
cmake ..
make firmware
```

5. Flash to PortaPack:
- Copy `firmware/portapack-h1_h2-mayhem.bin` to SD card
- Update through PortaPack bootloader

### Method 2: Pre-Built Binary (Future)

Pre-built firmware images with RF Scanner will be available in releases.

## Usage

### Quick Start

1. **Power on PortaPack**
2. Navigate to **Apps** > **RF Scanner**
3. Select scan band (or use Full Scan)
4. Adjust RSSI threshold (-120 to -30 dBm, default: -80)
5. Press **START** to begin scanning

### Interface

```
┌─────────────────────────────┐
│ RF Device Scanner           │
├─────────────────────────────┤
│ Frequency: 154.570 MHz      │
│ RSSI: -65 dBm               │
│ [====RSSI METER====]        │
│ Detections: 12              │
│                             │
│ Last Device: LAPD           │
│ Main Dispatch               │
│                             │
│ Band: [VHF Low ▼]           │
│ Threshold: [-80 dBm]        │
│                             │
│ [START] [CLEAR] [SAVE]      │
├─────────────────────────────┤
│ Console Output:             │
│ > LAPD @ 154.57 MHz         │
│ > LAFD @ 154.28 MHz         │
│ > CHP @ 42.34 MHz           │
└─────────────────────────────┘
```

### Controls

- **START/STOP Button**: Begin/end scanning
- **CLEAR Button**: Clear detection history
- **SAVE Button**: Save detections to SD card
- **Band Selector**: Choose frequency range
- **Threshold Field**: Adjust minimum RSSI for detection

### Scan Settings

**Recommended Settings by Use Case:**

| Use Case | Band | Threshold | Notes |
|----------|------|-----------|-------|
| General awareness | Full Scan | -80 dBm | Balanced detection |
| Police activity | VHF Low | -70 dBm | Focus on police bands |
| Fire/EMS | VHF Low | -70 dBm | Emergency services |
| Highway patrol | VHF Low | -80 dBm | CHP operates 42 MHz |
| Trunked systems | 800 MHz | -60 dBm | Strong signals only |
| FirstNet | 700 MHz | -65 dBm | LTE requires higher RSSI |

### Saving Detections

Press **SAVE** to create a log file on the SD card:

**File location**: `/RFSCANS/RFSCN_YYYYMMDD_HHMM.TXT`

**Example output:**
```
RF Scanner Detections
====================

Freq: 154.5700 MHz | RSSI: -65 dBm | Confidence: 85%
Device: LAPD
Description: Main Dispatch

Freq: 154.2800 MHz | RSSI: -72 dBm | Confidence: 75%
Device: LAFD
Description: Dispatch North

Freq: 42.3400 MHz | RSSI: -68 dBm | Confidence: 80%
Device: CHP
Description: Los Angeles Dispatch
```

## Detection Algorithm

### How It Works

1. **Frequency Scanning**: Sweeps through selected band in 12.5-25 kHz steps
2. **Signal Detection**: Captures signals above RSSI threshold
3. **Database Matching**: Compares detected frequency to known database (±25 kHz tolerance)
4. **Confidence Scoring**:
   - Base: 50%
   - RSSI bonus: +10-30% (stronger signal = higher confidence)
   - Frequency match: +20% (exact match)
5. **History Tracking**: Requires 3+ detections for confirmation
6. **Logging**: Records device type, frequency, RSSI, and timestamp

### Signal Strength Guide

| RSSI Level | Distance Estimate | Detection Reliability |
|------------|-------------------|----------------------|
| -40 to -50 dBm | Very close (<100m) | Excellent |
| -50 to -70 dBm | Near (100-500m) | Very good |
| -70 to -85 dBm | Medium (500m-1km) | Good |
| -85 to -100 dBm | Far (1-3km) | Fair |
| < -100 dBm | Very far (>3km) | Poor |

*Note: Actual range varies based on transmission power, antenna, terrain, and interference*

## Use Cases

### Legitimate Uses

- **Educational**: Learn about public safety radio systems
- **Security Research**: Authorized RF spectrum analysis
- **Emergency Monitoring**: Monitor emergency services during disasters
- **Ham Radio**: Identify and avoid busy public safety frequencies
- **Network Planning**: Assess spectrum utilization for interference avoidance
- **Situational Awareness**: Personal safety and awareness of nearby emergency activity

### Prohibited Uses

- **DO NOT** use to evade law enforcement
- **DO NOT** interfere with emergency communications
- **DO NOT** transmit on public safety frequencies without authorization
- **DO NOT** use for stalking, harassment, or illegal surveillance
- **DO NOT** record or publish private communications

## Legal Considerations

### United States Law

**Passive RF monitoring is generally legal** under the following conditions:

**Legal** (47 USC § 302a):
- Receiving unencrypted radio signals
- Using a scanner for education or hobby
- Monitoring public safety frequencies (listening only)
- Emergency monitoring during disasters
- Ham radio operators avoiding interference

**Illegal** (18 USC § 2511):
- Using scanned information to commit a crime
- Interfering with public safety operations
- Transmitting on frequencies without authorization
- Recording cellular phone calls
- Divulging contents of private communications

**State Laws Vary**:
- Some states restrict mobile use of scanners in vehicles
- Commercial use may require additional licensing
- Check your local regulations

**Bottom Line**: This tool is for **educational and awareness purposes**. The HackRF is **receive-only** in this application and poses no interference to public safety operations.

## Technical Details

### Scanning Performance

- **Scan Rate**: ~20-50 frequencies per second (depends on dwell time)
- **Frequency Resolution**: 25 kHz steps (configurable to 12.5 kHz)
- **RSSI Range**: -120 to -30 dBm
- **Detection Latency**: 50-150ms per frequency
- **Memory Usage**: ~50KB (detection storage)
- **SD Card**: Required for logging

### Frequency Tolerance

The scanner matches frequencies within **±25 kHz** of known entries to account for:
- Frequency drift in older equipment
- Adjacent channel detection
- Scanner tuning accuracy

### Database Coverage

| Category | Entries | Priority |
|----------|---------|----------|
| Law Enforcement | 80+ | High |
| Fire & EMS | 30+ | High |
| Federal Agencies | 20+ | High |
| Trunked Systems | 50+ | Medium |
| Transportation | 15+ | Medium |
| Airports | 10+ | Medium |
| Surveillance | 15+ | Medium |
| Schools | 10+ | Low |

**Total**: 300+ frequencies across 11 categories

## Advanced Features

### Signal History Tracking

The app maintains history for up to 100 unique signals:
- **RSSI History**: Last 10 measurements per frequency
- **Detection Count**: Total times signal was detected
- **Last Seen**: Timestamp of most recent detection
- **Trend Analysis**: Identify persistent vs. transient signals

### Trunked Radio Detection

For trunked systems (800 MHz, LA-RICS):
- Detects **control channels** (coordination frequencies)
- Identifies trunked system type
- Higher confidence for known control channel frequencies

### FirstNet LTE Detection

FirstNet (Band 14, 700 MHz) uses LTE technology:
- Detects broadband public safety network activity
- Separate uplink (773-776 MHz) and downlink (758-761 MHz) tracking
- Requires higher RSSI threshold due to spread spectrum

## Troubleshooting

### No Detections

**Possible Causes:**
1. RSSI threshold too high → Lower to -90 or -100 dBm
2. Wrong band selected → Try Full Scan mode
3. No activity in area → Normal, wait for activity
4. Antenna issue → Check antenna connection
5. Interference → Move to different location

### Too Many False Detections

**Solutions:**
1. Increase RSSI threshold to -70 or -60 dBm
2. Use specific band instead of Full Scan
3. Increase minimum detections (requires code change)
4. Move away from sources of interference (Wi-Fi, cellular)

### Weak Signals

**Improvements:**
1. Use higher-gain antenna
2. Raise antenna position (higher = better)
3. Move to open area (avoid buildings)
4. Scan during active periods (weekday daytime for business, evening for patrols)

### Battery Life

- **Active Scanning**: ~2-4 hours on battery
- **Recommendations**: Use USB power bank, Reduce LCD brightness, Disable unused peripherals

## Development

### Customization

To add custom frequencies, edit `freq_database.hpp`:

```cpp
// Add to appropriate array
static const FreqEntry freq_db_custom[] = {
    {162550000, "NOAA", "Weather Radio", 3},
    {156800000, "Marine", "VHF Channel 16", 4},
    // ... more entries
};
```

### Future Enhancements

- [ ] Audio demodulation (AM/FM/NFM)
- [ ] Waterfall display
- [ ] GPS logging integration
- [ ] Bluetooth alert forwarding
- [ ] Trunked radio following
- [ ] P25 decoding (if legal in your jurisdiction)
- [ ] Signal recording
- [ ] Web interface for configuration

## Credits

**Inspired by**: ESP32 Wireless Device Scanner project

**Built for**: HackRF PortaPack Mayhem firmware

**Frequency Data**: Compiled from public sources including:
- RadioReference.com
- FCC database
- Local public safety publications
- Ham radio frequency coordinators

## Disclaimer

This application is provided for **educational and research purposes only**.

**The authors and contributors:**
- Do NOT encourage illegal use of this software
- Are NOT responsible for misuse of this application
- Do NOT condone interference with public safety operations
- Are NOT liable for any consequences of use

**Users are solely responsible** for complying with all applicable federal, state, and local laws regarding RF monitoring.

**When in doubt, don't**. If you're not sure if something is legal, consult a lawyer.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL-2.0), consistent with PortaPack Mayhem firmware.

See LICENSE file for details.

## Support

- **Issues**: Report bugs via GitHub Issues
- **Discussions**: Join PortaPack Discord community
- **Updates**: Watch this repository for new frequency additions

---

**Version**: 1.0.0
**Last Updated**: January 2024
**Compatibility**: PortaPack Mayhem (latest)

**Stay safe. Stay legal. Stay curious.**
