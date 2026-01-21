# Quick Start Guide - RF Device Scanner

Get scanning in 5 minutes!

## Prerequisites

- HackRF One + PortaPack (H1, H2, H2M4, or H4M)
- PortaPack Mayhem firmware installed
- SD card inserted
- Antenna connected
- Battery or USB power

## Installation

### Option 1: Use Pre-Built Firmware (Recommended)

1. Download the latest firmware from Releases
2. Copy `portapack-mayhem.bin` to SD card root
3. Power off PortaPack
4. Power on while holding DFU button (or use bootloader)
5. Select firmware update
6. Wait for flash to complete
7. Reboot

### Option 2: Build from Source

```bash
# Clone this repo
git clone https://github.com/yourusername/hackrf-portapack-rfscanner.git
cd hackrf-portapack-rfscanner

# Clone PortaPack Mayhem
git clone https://github.com/eried/portapack-mayhem.git

# Copy files
cp rf_scanner.* portapack-mayhem/firmware/application/
cp freq_database.hpp portapack-mayhem/firmware/application/

# Build (requires Docker or ARM toolchain)
cd portapack-mayhem
docker run --rm -v $(pwd):/havoc portapack-dev
```

## First Scan - Step by Step

### Step 1: Launch App

1. Power on PortaPack
2. Tap **Apps** (toolbox icon)
3. Scroll to **RF Scanner**
4. Tap to launch

### Step 2: Configure Scan

**For beginners, use these settings:**

```
Band: Full Scan
Threshold: -80 dBm
```

This will scan all frequencies and detect strong signals.

### Step 3: Start Scanning

1. Tap **START** button
2. Watch the frequency display update
3. Console will show detections as they occur

### Step 4: Interpret Results

When a signal is detected, you'll see:

```
Last Device: LAPD
Description: Main Dispatch
Detections: 1
```

The console shows a running log:
```
> LAPD @ 154.57 MHz
> LAFD @ 154.28 MHz
```

### Step 5: Save Results (Optional)

1. Tap **STOP** to pause scanning
2. Tap **SAVE** to write log to SD card
3. File saved to `/RFSCANS/RFSCN_DATE_TIME.TXT`

## Common Scenarios

### Scenario 1: Monitor Police Activity

**Goal**: Detect nearby police radio activity

**Settings:**
- Band: **VHF Low** (150-174 MHz)
- Threshold: **-70 dBm**

**Expected Results:**
- LAPD, LASD, CHP dispatch channels
- Strong signals within 1 km
- Weak signals up to 3 km

**What to look for:**
- Frequent detections = active patrols nearby
- SWAT/Tactical frequencies = special operations
- Air Support = helicopter activity

### Scenario 2: Emergency Services Monitoring

**Goal**: Monitor fire and EMS during emergency

**Settings:**
- Band: **VHF Low** (150-174 MHz)
- Threshold: **-80 dBm**

**Expected Results:**
- LAFD dispatch and tactical channels
- EMS/Ambulance frequencies
- Hospital link frequencies

**What to look for:**
- Multiple channels active = large incident
- CalFire frequencies = wildfire response

### Scenario 3: Highway Patrol Detection

**Goal**: Detect CHP activity on freeways

**Settings:**
- Band: **VHF Low** (150-174 MHz)
- Threshold: **-75 dBm**

**Expected Results:**
- CHP operates on 42 MHz band
- Detections increase near freeway
- Air ops indicate aircraft enforcement

### Scenario 4: Federal Agency Detection

**Goal**: Detect FBI, DEA, or other federal activity

**Settings:**
- Band: **VHF High** (174-216 MHz)
- Threshold: **-70 dBm**

**Expected Results:**
- FBI, DEA, ATF, Secret Service frequencies
- Often encrypted or intermittent
- Higher confidence when detected

### Scenario 5: Trunked Radio Systems

**Goal**: Detect modern trunked public safety systems

**Settings:**
- Band: **800 MHz** (806-869 MHz)
- Threshold: **-60 dBm**

**Expected Results:**
- Control channel detections
- LA-RICS system identification
- Strong signals required (nearby towers)

## Tips for Better Results

### Antenna Positioning

- **Height matters**: Raise antenna as high as possible
- **Line of sight**: Outdoor scanning works best
- **Avoid metal**: Stay away from metal structures that block signals
- **Orientation**: Vertical for VHF/UHF

### Timing

**Best times to scan:**
- **Weekday mornings (7-9 AM)**: Rush hour, high patrol activity
- **Weekday evenings (5-8 PM)**: Rush hour, shift changes
- **Weekend nights**: Increased DUI patrols
- **Special events**: Concerts, sports, protests

**Worst times:**
- **Late night (2-6 AM)**: Minimal activity
- **Early Sunday mornings**: Lowest traffic

### Location

**Best locations:**
- **Hills/elevated areas**: Better line of sight
- **Near freeways**: CHP activity
- **Near airports**: Airport police, fire
- **Downtown**: Multiple agencies active
- **Near government buildings**: Federal agencies

**Avoid:**
- **Basements/underground**: Poor signal reception
- **Dense urban canyons**: Buildings block signals
- **Near strong RF sources**: Interference from Wi-Fi, cellular towers

## Understanding the Display

### Main Screen

```
Frequency: 154.570 MHz     ← Current scanning frequency
RSSI: -65 dBm              ← Signal strength
[=====RSSI METER=====]     ← Visual signal strength
Detections: 12             ← Total detections this session
Last Device: LAPD          ← Most recent detection
Main Dispatch              ← Description
```

### RSSI Meter

```
[===                ]  -90 dBm (Weak - far away)
[========           ]  -75 dBm (Medium - moderate distance)
[==============     ]  -60 dBm (Strong - nearby)
[===================]  -45 dBm (Very strong - very close)
```

### Console Output

Real-time log of all detections:

```
> LAPD @ 154.57 MHz        ← Detection 1
> LAFD @ 154.28 MHz        ← Detection 2
> CHP @ 42.34 MHz          ← Detection 3
```

Scroll with encoder to view history.

## Troubleshooting

### Problem: No Detections

**Solution checklist:**
- [ ] Check antenna is connected
- [ ] Lower threshold to -90 or -100 dBm
- [ ] Try Full Scan mode
- [ ] Move to outdoor location
- [ ] Wait 2-3 minutes (activity may be low)
- [ ] Try different time of day

### Problem: Too Many Detections

**Solution:**
- Increase threshold to -65 or -60 dBm
- Use specific band (not Full Scan)
- Move away from interference sources

### Problem: Scanning Stops

**Solution:**
- Check battery level
- Ensure SD card is inserted
- Restart PortaPack
- Reflash firmware if persists

### Problem: Weak Signals

**Solution:**
- Use better antenna (high-gain VHF/UHF)
- Raise antenna higher
- Move to elevated location
- Scan during active hours

## Next Steps

Once you're comfortable with basic scanning:

1. **Experiment with bands**: Try each band individually
2. **Adjust threshold**: Find optimal settings for your area
3. **Save logs**: Build a database of local frequencies
4. **Learn patterns**: Identify regular patrol patterns
5. **Explore advanced**: Research trunked systems, P25, etc.

## Safety & Legal

**Remember:**
- **LISTEN ONLY** - Never transmit on these frequencies
- **Don't interfere** with emergency operations
- **Don't use for illegal purposes**
- **Check local laws** regarding scanner use in vehicles

## Resources

- **RadioReference.com**: Frequency database lookup
- **PortaPack Discord**: Community support
- **HackRF Wiki**: Technical documentation
- **FCC Database**: Official frequency allocations

## Support

- **GitHub Issues**: Report bugs
- **Discord**: Community help
- **Email**: support@example.com

---

**Happy scanning! Stay safe, stay legal, stay curious.**
