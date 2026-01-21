# Frequency Database Documentation

## Overview

The RF Device Scanner includes a comprehensive database of **300+ frequencies** used by government, law enforcement, emergency services, and public safety agencies in Los Angeles County and Southern California.

## Database Statistics

| Category | Entries | Priority Level | Frequency Ranges |
|----------|---------|----------------|------------------|
| Law Enforcement | 80+ | High | VHF, UHF, 800 MHz |
| Fire & EMS | 30+ | High | VHF, UHF |
| Federal Agencies | 20+ | High | VHF High, UHF |
| Trunked Systems | 50+ | Medium | 700, 800 MHz |
| Transportation | 15+ | Medium | VHF, UHF |
| Airports | 10+ | Medium | VHF, UHF, Air |
| Surveillance | 15+ | Medium | UHF, 900 MHz |
| Schools | 10+ | Low | UHF |
| FirstNet | 10+ | High | 700 MHz |
| ALPR/Traffic | 10+ | Medium | 900 MHz |
| Misc | 20+ | Low | Various |

**Total Database Size**: 300+ frequencies across 11 categories

## Frequency Bands Explained

### VHF Low Band (150-174 MHz)

**Primary Users**: Local police, fire, EMS, business

**Characteristics**:
- Excellent range (up to 10+ miles)
- Good building penetration
- Widely used by municipal agencies
- Analog FM (mostly)

**Notable Frequencies**:
```
150-156 MHz: Public Safety Pool
154.570: LAPD Main Dispatch
154.280: LAFD Dispatch North
155.475: LASD North Patrol
42.340:  CHP Los Angeles (LOW VHF)
```

### VHF High Band (174-216 MHz)

**Primary Users**: Government, military, federal agencies

**Characteristics**:
- Good range (5-8 miles)
- Less crowded than VHF Low
- Often used for tactical/sensitive operations
- Mix of analog and digital

**Notable Frequencies**:
```
163.000-174.000: Federal Government
167.650: FBI Tactical 1
165.212: DEA Operations
166.462: ATF Operations
```

### UHF-T Band (450-470 MHz)

**Primary Users**: Public safety, business, government

**Characteristics**:
- Moderate range (3-5 miles)
- Common for tactical and special units
- Digital capable
- Less interference

**Notable Frequencies**:
```
453.550: LASD Special Enforcement
460.225: LAPD SWAT Primary
460.450: LASD Metro Detail
460.525: BHPD Dispatch
```

### UHF Public Safety (470-512 MHz)

**Primary Users**: Government exclusive

**Characteristics**:
- Reserved for government use only
- Moderate range (3-5 miles)
- High-quality communications
- Often digital/encrypted

**Notable Frequencies**:
```
470-512 MHz: Government Pool
Varies by agency and region
```

### 700 MHz Band (700-800 MHz)

**Primary Users**: FirstNet (LTE Public Safety Network)

**Characteristics**:
- **Band 14** (758-769 MHz downlink, 788-799 MHz uplink)
- Broadband data (not voice)
- Nationwide public safety network
- Encrypted by default
- Requires LTE decoding (advanced)

**Notable Frequencies**:
```
758-761: FirstNet Downlink
773-776: FirstNet Uplink
770-773: LA-RICS Control
```

### 800 MHz Band (806-869 MHz)

**Primary Users**: Trunked radio systems (P25, Motorola, EDACS)

**Characteristics**:
- **Primary band for modern trunked systems**
- Digital voice (P25, DMR)
- Often encrypted
- Control channels coordinate voice traffic
- Shorter range (2-4 miles per site)

**Notable Frequencies**:
```
851.512: Trunked Control Channel
856.237: LAPD Trunked System
856.787: LASD Trunked System
806-869: Full 800 MHz public safety band
```

### 900 MHz Band (896-960 MHz)

**Primary Users**: Cellular, trunked systems, surveillance

**Characteristics**:
- Mix of public safety and commercial
- Shorter range (1-3 miles)
- High frequency = more line-of-sight dependent
- Used for surveillance equipment

**Notable Frequencies**:
```
902-928: ISM Band (ALPR, sensors, cameras)
915.000: Traffic sensors
902.500: ALPR (License Plate Readers)
```

## Agency Frequency Lists

### LAPD (Los Angeles Police Department)

| Frequency | Mode | Description | Priority |
|-----------|------|-------------|----------|
| 154.570 MHz | NFM | Main Dispatch | High |
| 154.680 MHz | NFM | Tactical 1 | High |
| 154.755 MHz | NFM | Detectives | Medium |
| 154.845 MHz | NFM | Air Support (Air-to-Ground) | High |
| 155.475 MHz | NFM | Command Channel | Medium |
| 460.225 MHz | NFM | SWAT Primary | High |
| 460.275 MHz | NFM | SWAT Secondary | High |
| 856.237 MHz | P25 | Trunked System (Digital) | Medium |

**Notes**:
- LAPD primarily uses conventional FM for dispatch
- Trunked system is supplementary
- Air Support also uses aviation band (123.075 MHz)

### LASD (Los Angeles County Sheriff)

| Frequency | Mode | Description | Priority |
|-----------|------|-------------|----------|
| 155.475 MHz | NFM | North Patrol | High |
| 155.505 MHz | NFM | East Patrol | High |
| 155.535 MHz | NFM | South Patrol | High |
| 155.565 MHz | NFM | West Patrol | High |
| 155.730 MHz | NFM | Century Station | Medium |
| 155.760 MHz | NFM | Lancaster Station | Medium |
| 453.550 MHz | NFM | Special Enforcement | High |
| 460.450 MHz | NFM | Metro Detail | High |
| 856.787 MHz | P25 | Trunked System | Medium |

**Notes**:
- Large coverage area (entire LA County)
- Divided into patrol regions
- Each station may have dedicated frequencies

### CHP (California Highway Patrol)

| Frequency | Mode | Description | Priority |
|-----------|------|-------------|----------|
| 42.340 MHz | NFM | Los Angeles Dispatch | High |
| 42.380 MHz | NFM | Central LA | High |
| 42.420 MHz | NFM | West LA | High |
| 42.460 MHz | NFM | South LA | High |
| 42.500 MHz | NFM | East LA | High |
| 42.860 MHz | NFM | Air Operations | High |
| 44.620 MHz | NFM | Tactical | Medium |

**Notes**:
- CHP uses **low VHF band** (42 MHz)
- Requires antenna optimized for 40-50 MHz
- Excellent range due to low frequency
- Air ops coordinate with fixed-wing aircraft

### LAFD (Los Angeles Fire Department)

| Frequency | Mode | Description | Priority |
|-----------|------|-------------|----------|
| 154.280 MHz | NFM | Dispatch North | High |
| 154.190 MHz | NFM | Dispatch South | High |
| 154.145 MHz | NFM | Dispatch Central | High |
| 154.220 MHz | NFM | Tactical (Fireground) | High |
| 154.265 MHz | NFM | Command | Medium |
| 453.050 MHz | NFM | Air Operations (Helicopters) | High |
| 460.550 MHz | NFM | EMS Primary | High |

**Notes**:
- Multiple dispatch zones for large city coverage
- Tactical channels used at fire scenes
- Air ops use water-dropping helicopters

### Federal Agencies

| Agency | Frequency | Mode | Description | Priority |
|--------|-----------|------|-------------|----------|
| FBI | 167.650 MHz | NFM | Tactical 1 | High |
| FBI | 167.850 MHz | NFM | Tactical 2 | High |
| FBI | 163.087 MHz | NFM | Surveillance | High |
| DEA | 165.212 MHz | NFM | Operations | High |
| DEA | 418.100 MHz | NFM | Tactical | High |
| ATF | 166.462 MHz | NFM | Operations | High |
| USMS | 162.637 MHz | NFM | Marshals Ops | High |
| USSS | 165.375 MHz | NFM | Secret Service Protection | High |
| USSS | 407.800 MHz | NFM | Tactical | High |
| DHS | 406.200 MHz | NFM | Homeland Security Ops | High |

**Notes**:
- Federal frequencies often encrypted
- VHF High band (162-174 MHz) is federal pool
- UHF (406-420 MHz) used for tactical operations
- May use trunked P25 systems

### LA-RICS (Regional Interoperability)

| Frequency | Mode | Description | Priority |
|-----------|------|-------------|----------|
| 770.000 MHz | P25 | Control Channel | High |
| 771.000 MHz | P25 | Control Channel | High |
| 772.000 MHz | P25 | Control Channel | High |
| 773.000 MHz | P25 | Control Channel | High |

**Notes**:
- **LA-RICS** = LA Regional Interoperable Communications System
- 700 MHz P25 Phase 2 trunked system
- Shared by multiple agencies
- Requires P25 decoder (not included in basic scanner)

### FirstNet (LTE Public Safety)

| Frequency | Type | Description | Priority |
|-----------|------|-------------|----------|
| 758-761 MHz | LTE | Band 14 Downlink | High |
| 773-776 MHz | LTE | Band 14 Uplink | High |

**Notes**:
- **Nationwide** public safety broadband network
- LTE technology (not traditional radio)
- Encrypted end-to-end
- Used for data, video, apps (not voice radio)
- Detecting LTE signals shows network presence

## Detection Tips by Agency

### How to Detect LAPD

**Best Band**: VHF Low (150-174 MHz)
**Key Frequencies**: 154.570, 154.680, 154.845
**RSSI Threshold**: -80 dBm
**Expected Range**: Up to 2-3 km

**Interpretation**:
- **154.570 active**: Main dispatch in use (normal operations)
- **460.225 active**: SWAT deployment (special operation)
- **154.845 active**: Air support (helicopter in area)

### How to Detect LAFD

**Best Band**: VHF Low (150-174 MHz)
**Key Frequencies**: 154.280, 154.190, 154.220
**RSSI Threshold**: -75 dBm
**Expected Range**: Up to 3-4 km

**Interpretation**:
- **Multiple dispatch channels active**: Large incident
- **154.220 sustained activity**: Active fire scene
- **453.050 active**: Helicopter water drops (wildfire)

### How to Detect CHP

**Best Band**: VHF Low (42-50 MHz)
**Key Frequencies**: 42.340, 42.380, 42.860
**RSSI Threshold**: -70 dBm
**Expected Range**: Up to 5-10 km (low frequency)

**Interpretation**:
- **42.xxx active**: Highway patrol in area
- **42.860 active**: Aircraft enforcement (speed monitoring)
- **Strong signal near freeway**: Traffic stop or pursuit

### How to Detect Federal Agents

**Best Band**: VHF High (162-174 MHz)
**Key Frequencies**: 167.650 (FBI), 165.212 (DEA)
**RSSI Threshold**: -70 dBm
**Expected Range**: 2-4 km

**Interpretation**:
- **167.650 sustained**: FBI operation in progress
- **165.212 active**: DEA drug enforcement activity
- **Often encrypted**: May detect carrier without audio

## Frequency Allocation Charts

### VHF Public Safety Allocation (US)

```
150.000 - 150.995: Business/Industrial
151.000 - 154.000: Public Safety (State/Local)
154.000 - 156.000: Public Safety (Primary)
156.000 - 157.000: Marine/Maritime
157.000 - 158.000: Mixed use
158.000 - 162.000: Federal Government
162.000 - 174.000: Federal Government
174.000 - 216.000: TV Broadcast (Channels 7-13)
```

### UHF Public Safety Allocation

```
450.000 - 470.000: UHF-T Band (Public Safety)
470.000 - 512.000: UHF Public Safety (Gov Only)
512.000 - 698.000: TV Broadcast (UHF)
698.000 - 806.000: LTE Cellular
806.000 - 869.000: 800 MHz Public Safety
869.000 - 896.000: Cellular
896.000 - 901.000: Cellular
901.000 - 902.000: Unlicensed
902.000 - 928.000: ISM Band (Industrial, Scientific, Medical)
```

## Adding Custom Frequencies

To add your own frequencies, edit `freq_database.hpp`:

### Template

```cpp
static const FreqEntry freq_db_custom[] = {
    {FREQUENCY_HZ, "AGENCY", "Description", PRIORITY},
    // Example:
    {162550000, "NOAA", "Weather Radio", 3},
    {156800000, "Marine", "VHF Ch 16 (Distress)", 5},
};
```

### Priority Levels

- **5**: Critical (emergency, tactical, SWAT)
- **4**: High (dispatch, operations)
- **3**: Medium (support, training)
- **2**: Low (maintenance, admin)
- **1**: Minimal (misc, non-critical)

### Frequency Format

Frequencies must be in **Hertz** (not MHz):

| MHz | Hertz | Example |
|-----|-------|---------|
| 154.570 | 154570000 | LAPD Dispatch |
| 42.34 | 42340000 | CHP |
| 856.2375 | 856237500 | 800 MHz (12.5 kHz steps) |

### Rebuilding Database

After editing `freq_database.hpp`:

1. Save file
2. Rebuild firmware
3. Flash to PortaPack
4. Test with RF Scanner app

## Database Sources

Frequencies compiled from:

- **RadioReference.com**: Community database
- **FCC ULS Database**: Official license records
- **Local frequency guides**: Published by ham radio clubs
- **Public safety publications**: Agency frequency lists
- **Field scanning**: Verified by actual detection

## Database Updates

This database is maintained as an open-source project. To contribute:

1. Fork repository
2. Add frequencies to `freq_database.hpp`
3. Test with actual scanning
4. Submit pull request with documentation

### Contribution Guidelines

**Required information**:
- Frequency (verified)
- Agency/Device type
- Description
- Source (where did you find this frequency?)
- Location (city, county, state)
- Priority level

**Example contribution**:
```
Frequency: 460.625 MHz
Agency: Ventura County Sheriff
Description: Dispatch
Source: RadioReference.com, verified by scanning
Location: Ventura County, CA
Priority: 4 (High)
```

## Legal Notice

All frequencies in this database are:
- Publicly available information
- Published by regulatory agencies (FCC)
- Compiled from open-source databases
- Intended for educational use only

**Listening to these frequencies is legal in most jurisdictions**, but check your local laws. **Never transmit on these frequencies without proper authorization.**

## Disclaimer

This database is provided **as-is** for educational purposes.

- Frequencies may change without notice
- Some frequencies may be inactive or deprecated
- Encryption may prevent decoding
- Trunked systems require advanced decoding

**Accuracy**: We strive for accuracy but cannot guarantee all frequencies are correct or current.

---

**Last Updated**: January 2024
**Database Version**: 1.0.0
**Entries**: 300+
**Coverage**: Los Angeles County, Southern California
