/*
 * RF Device Scanner - Frequency Database
 *
 * Comprehensive database of public safety and government frequencies
 * Focused on Los Angeles County and Southern California
 *
 * Frequency bands covered:
 * - VHF Low Band (150-174 MHz): Public Safety, Business
 * - VHF High Band (174-216 MHz): Government, Military
 * - UHF-T Band (450-470 MHz): Public Safety, Business
 * - UHF Public Safety (470-512 MHz): Government exclusive
 * - 700 MHz (764-776, 794-806 MHz): LTE Public Safety (FirstNet)
 * - 800 MHz (806-869 MHz): Trunked Radio Systems
 * - 900 MHz (896-960 MHz): Cellular, Trunked systems
 */

#ifndef __FREQ_DATABASE_H__
#define __FREQ_DATABASE_H__

#include <cstdint>
#include <cstddef>

// Frequency entry structure
struct FreqEntry {
    uint32_t frequency;        // Frequency in Hz
    const char* device_type;   // Type of device (e.g., "LAPD", "LAFD")
    const char* description;   // Detailed description
    uint8_t priority;          // Priority level (1-5, 5 = highest)
};

// LA County Police Departments
static const FreqEntry freq_db_police[] = {
    // LAPD (Los Angeles Police Department)
    {154570000, "LAPD", "Main Dispatch", 5},
    {154680000, "LAPD", "Tactical", 5},
    {154755000, "LAPD", "Detectives", 4},
    {154845000, "LAPD", "Air Support", 5},
    {155475000, "LAPD", "Command", 4},
    {155535000, "LAPD", "Operations", 4},
    {460225000, "LAPD", "SWAT Primary", 5},
    {460275000, "LAPD", "SWAT Secondary", 5},
    {856237500, "LAPD", "Trunked System", 4},
    {857487500, "LAPD", "Trunked System", 4},

    // LASD (Los Angeles Sheriff's Department)
    {155475000, "LASD", "North Patrol", 5},
    {155505000, "LASD", "East Patrol", 5},
    {155535000, "LASD", "South Patrol", 5},
    {155565000, "LASD", "West Patrol", 5},
    {155730000, "LASD", "Century Station", 4},
    {155760000, "LASD", "Lancaster Station", 4},
    {154890000, "LASD", "Emergency Services", 5},
    {453550000, "LASD", "Special Enforcement", 5},
    {460450000, "LASD", "Metro Detail", 5},
    {856787500, "LASD", "Trunked System", 4},

    // California Highway Patrol (CHP)
    {42340000, "CHP", "Los Angeles Dispatch", 5},
    {42380000, "CHP", "Central LA", 5},
    {42420000, "CHP", "West LA", 5},
    {42460000, "CHP", "South LA", 5},
    {42500000, "CHP", "East LA", 5},
    {42860000, "CHP", "Air Operations", 5},
    {44620000, "CHP", "Tactical", 4},
    {159090000, "CHP", "Car-to-Car", 3},

    // Beverly Hills Police
    {460525000, "BHPD", "Dispatch", 4},
    {460550000, "BHPD", "Tactical", 4},
    {155610000, "BHPD", "Operations", 3},

    // Santa Monica Police
    {460450000, "SMPD", "Dispatch", 4},
    {460475000, "SMPD", "Tactical", 4},
    {155610000, "SMPD", "Beach Patrol", 3},

    // Long Beach Police
    {460375000, "LBPD", "Dispatch", 4},
    {460425000, "LBPD", "Tactical", 4},
    {155670000, "LBPD", "Harbor Patrol", 3},

    // Pasadena Police
    {460500000, "PPD", "Dispatch", 4},
    {460525000, "PPD", "Tactical", 4},

    // Glendale Police
    {154905000, "GPD", "Dispatch", 4},
    {154980000, "GPD", "Tactical", 4},

    // Burbank Police
    {155760000, "BPD", "Dispatch", 4},
    {460425000, "BPD", "Tactical", 4},

    // Torrance Police
    {460550000, "TPD", "Dispatch", 4},
    {460575000, "TPD", "Tactical", 4},
};

// Fire Departments
static const FreqEntry freq_db_fire[] = {
    // LAFD (Los Angeles Fire Department)
    {154280000, "LAFD", "Dispatch North", 5},
    {154190000, "LAFD", "Dispatch South", 5},
    {154145000, "LAFD", "Dispatch Central", 5},
    {154220000, "LAFD", "Tactical", 5},
    {154265000, "LAFD", "Command", 4},
    {154310000, "LAFD", "Arson Investigation", 4},
    {453050000, "LAFD", "Air Operations", 5},
    {460550000, "LAFD", "EMS Primary", 5},

    // LACoFD (LA County Fire Department)
    {154175000, "LACoFD", "Dispatch", 5},
    {154265000, "LACoFD", "Tactical", 5},
    {154280000, "LACoFD", "Command", 4},
    {154340000, "LACoFD", "Training", 3},
    {460575000, "LACoFD", "EMS", 5},

    // CalFire (California Department of Forestry and Fire Protection)
    {151175000, "CalFire", "Air-to-Ground", 5},
    {154310000, "CalFire", "Dispatch", 5},
    {159450000, "CalFire", "Tactical", 5},
    {171425000, "CalFire", "Command", 4},

    // Beverly Hills Fire
    {154250000, "BHFD", "Dispatch", 4},
    {154295000, "BHFD", "Tactical", 4},

    // Santa Monica Fire
    {154205000, "SMFD", "Dispatch", 4},
    {154250000, "SMFD", "Tactical", 4},

    // Long Beach Fire
    {154130000, "LBFD", "Dispatch", 4},
    {154220000, "LBFD", "Tactical", 4},
};

// Emergency Medical Services
static const FreqEntry freq_db_ems[] = {
    {463025000, "EMS", "Hospital Link 1", 4},
    {463050000, "EMS", "Hospital Link 2", 4},
    {463075000, "EMS", "Hospital Link 3", 4},
    {463100000, "EMS", "Hospital Link 4", 4},
    {463125000, "EMS", "Hospital Link 5", 4},
    {463150000, "EMS", "Hospital Link 6", 4},
    {460550000, "EMS", "Ambulance Dispatch", 5},
    {460575000, "EMS", "Paramedic", 5},
    {155340000, "EMS", "MedStar", 4},
};

// Government & Federal Agencies
static const FreqEntry freq_db_government[] = {
    // FBI
    {167650000, "FBI", "Tactical 1", 5},
    {167850000, "FBI", "Tactical 2", 5},
    {163087500, "FBI", "Surveillance", 5},
    {406175000, "FBI", "Operations", 5},

    // DEA (Drug Enforcement Administration)
    {165212500, "DEA", "Operations", 5},
    {165287500, "DEA", "Surveillance", 5},
    {418100000, "DEA", "Tactical", 5},

    // ATF (Bureau of Alcohol, Tobacco, Firearms and Explosives)
    {166462500, "ATF", "Operations", 5},
    {408350000, "ATF", "Tactical", 5},

    // US Marshals
    {162637500, "USMS", "Operations", 5},
    {165537500, "USMS", "Fugitive Task Force", 5},

    // Secret Service
    {165375000, "USSS", "Protection", 5},
    {166212500, "USSS", "Surveillance", 5},
    {407800000, "USSS", "Tactical", 5},

    // Border Patrol
    {163087500, "BP", "Patrol", 4},
    {138112500, "BP", "Air Operations", 4},

    // ICE (Immigration and Customs Enforcement)
    {164637500, "ICE", "Operations", 4},
    {418500000, "ICE", "Enforcement", 4},

    // DHS (Department of Homeland Security)
    {406200000, "DHS", "Operations", 5},
    {418800000, "DHS", "Tactical", 5},
};

// Transportation & Traffic
static const FreqEntry freq_db_transportation[] = {
    // LA Metro
    {160710000, "Metro", "Rail Operations", 4},
    {160920000, "Metro", "Bus Dispatch", 4},
    {452950000, "Metro", "Security", 4},

    // Caltrans (California Department of Transportation)
    {151955000, "Caltrans", "Maintenance", 3},
    {453450000, "Caltrans", "Operations", 3},
    {460625000, "Caltrans", "TMC (Traffic Management)", 4},

    // Parking Enforcement
    {460125000, "Parking", "LA Enforcement", 3},
    {460150000, "Parking", "Citations", 3},
};

// Airport Security
static const FreqEntry freq_db_airports[] = {
    // LAX (Los Angeles International Airport)
    {460475000, "LAX PD", "Police Dispatch", 5},
    {460500000, "LAX PD", "Tactical", 5},
    {461600000, "LAX", "Operations", 4},
    {462925000, "LAX", "Fire/Rescue", 5},
    {131725000, "LAX", "Tower", 3},
    {132700000, "LAX", "Ground", 3},

    // Bob Hope Airport (Burbank)
    {460425000, "BUR PD", "Airport Police", 4},
    {132200000, "BUR", "Tower", 3},

    // Long Beach Airport
    {460375000, "LGB PD", "Airport Police", 4},
    {119300000, "LGB", "Tower", 3},
};

// Schools & Universities
static const FreqEntry freq_db_schools[] = {
    // LAUSD (Los Angeles Unified School District)
    {154600000, "LAUSD PD", "Dispatch", 4},
    {460625000, "LAUSD PD", "Tactical", 4},
    {464525000, "LAUSD", "Operations", 3},

    // UCLA Police
    {460225000, "UCLA PD", "Dispatch", 4},
    {460250000, "UCLA PD", "Tactical", 4},

    // USC Public Safety
    {460375000, "USC PS", "Dispatch", 4},
    {460400000, "USC PS", "Patrol", 4},
};

// Trunked Radio Systems (Control Channels)
static const FreqEntry freq_db_trunked[] = {
    // LA Regional Interoperable Communications System (LA-RICS)
    {770000000, "LA-RICS", "Control Channel", 5},
    {771000000, "LA-RICS", "Control Channel", 5},
    {772000000, "LA-RICS", "Control Channel", 5},
    {773000000, "LA-RICS", "Control Channel", 5},

    // 800 MHz Trunked Systems
    {851512500, "Trunked", "Control Channel", 4},
    {852512500, "Trunked", "Control Channel", 4},
    {853512500, "Trunked", "Control Channel", 4},
    {854512500, "Trunked", "Control Channel", 4},
    {856512500, "Trunked", "Control Channel", 4},
    {857512500, "Trunked", "Control Channel", 4},
    {858512500, "Trunked", "Control Channel", 4},
    {859512500, "Trunked", "Control Channel", 4},
};

// FirstNet (First Responder Network Authority) - LTE Public Safety
static const FreqEntry freq_db_firstnet[] = {
    {773000000, "FirstNet", "Band 14 Uplink", 5},
    {774000000, "FirstNet", "Band 14 Uplink", 5},
    {775000000, "FirstNet", "Band 14 Uplink", 5},
    {776000000, "FirstNet", "Band 14 Uplink", 5},
    {758000000, "FirstNet", "Band 14 Downlink", 5},
    {759000000, "FirstNet", "Band 14 Downlink", 5},
    {760000000, "FirstNet", "Band 14 Downlink", 5},
};

// License Plate Readers (ALPR) & Traffic Systems
static const FreqEntry freq_db_alpr[] = {
    {902500000, "ALPR", "License Plate Reader Link", 4},
    {903500000, "ALPR", "License Plate Reader Link", 4},
    {904500000, "ALPR", "License Plate Reader Link", 4},
    {915000000, "Traffic Sensor", "Loop Detector", 3},
    {916000000, "Traffic Sensor", "Speed Detection", 3},
    {917000000, "Traffic Camera", "Video Link", 3},
};

// Surveillance & Tactical
static const FreqEntry freq_db_surveillance[] = {
    {450025000, "Surveillance", "Mobile Surveillance", 4},
    {450050000, "Surveillance", "Body Camera Link", 4},
    {450075000, "Surveillance", "Dash Camera Link", 4},
    {851000000, "Surveillance", "Video Downlink", 4},
    {852000000, "Surveillance", "Video Downlink", 4},
    {900500000, "Surveillance", "Portable Camera", 3},
};

// Total database size
static constexpr size_t FREQ_DB_SIZE =
    sizeof(freq_db_police) / sizeof(FreqEntry) +
    sizeof(freq_db_fire) / sizeof(FreqEntry) +
    sizeof(freq_db_ems) / sizeof(FreqEntry) +
    sizeof(freq_db_government) / sizeof(FreqEntry) +
    sizeof(freq_db_transportation) / sizeof(FreqEntry) +
    sizeof(freq_db_airports) / sizeof(FreqEntry) +
    sizeof(freq_db_schools) / sizeof(FreqEntry) +
    sizeof(freq_db_trunked) / sizeof(FreqEntry) +
    sizeof(freq_db_firstnet) / sizeof(FreqEntry) +
    sizeof(freq_db_alpr) / sizeof(FreqEntry) +
    sizeof(freq_db_surveillance) / sizeof(FreqEntry);

// Search function to find frequency in database
// Returns nullptr if not found
inline const FreqEntry* find_frequency(uint32_t freq) {
    // Tolerance for frequency matching (±25 kHz)
    const uint32_t tolerance = 25000;

    // Search police frequencies
    for (size_t i = 0; i < sizeof(freq_db_police) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_police[i].frequency - tolerance &&
            freq <= freq_db_police[i].frequency + tolerance) {
            return &freq_db_police[i];
        }
    }

    // Search fire frequencies
    for (size_t i = 0; i < sizeof(freq_db_fire) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_fire[i].frequency - tolerance &&
            freq <= freq_db_fire[i].frequency + tolerance) {
            return &freq_db_fire[i];
        }
    }

    // Search EMS frequencies
    for (size_t i = 0; i < sizeof(freq_db_ems) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_ems[i].frequency - tolerance &&
            freq <= freq_db_ems[i].frequency + tolerance) {
            return &freq_db_ems[i];
        }
    }

    // Search government frequencies
    for (size_t i = 0; i < sizeof(freq_db_government) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_government[i].frequency - tolerance &&
            freq <= freq_db_government[i].frequency + tolerance) {
            return &freq_db_government[i];
        }
    }

    // Search transportation frequencies
    for (size_t i = 0; i < sizeof(freq_db_transportation) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_transportation[i].frequency - tolerance &&
            freq <= freq_db_transportation[i].frequency + tolerance) {
            return &freq_db_transportation[i];
        }
    }

    // Search airport frequencies
    for (size_t i = 0; i < sizeof(freq_db_airports) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_airports[i].frequency - tolerance &&
            freq <= freq_db_airports[i].frequency + tolerance) {
            return &freq_db_airports[i];
        }
    }

    // Search school frequencies
    for (size_t i = 0; i < sizeof(freq_db_schools) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_schools[i].frequency - tolerance &&
            freq <= freq_db_schools[i].frequency + tolerance) {
            return &freq_db_schools[i];
        }
    }

    // Search trunked frequencies
    for (size_t i = 0; i < sizeof(freq_db_trunked) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_trunked[i].frequency - tolerance &&
            freq <= freq_db_trunked[i].frequency + tolerance) {
            return &freq_db_trunked[i];
        }
    }

    // Search FirstNet frequencies
    for (size_t i = 0; i < sizeof(freq_db_firstnet) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_firstnet[i].frequency - tolerance &&
            freq <= freq_db_firstnet[i].frequency + tolerance) {
            return &freq_db_firstnet[i];
        }
    }

    // Search ALPR frequencies
    for (size_t i = 0; i < sizeof(freq_db_alpr) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_alpr[i].frequency - tolerance &&
            freq <= freq_db_alpr[i].frequency + tolerance) {
            return &freq_db_alpr[i];
        }
    }

    // Search surveillance frequencies
    for (size_t i = 0; i < sizeof(freq_db_surveillance) / sizeof(FreqEntry); i++) {
        if (freq >= freq_db_surveillance[i].frequency - tolerance &&
            freq <= freq_db_surveillance[i].frequency + tolerance) {
            return &freq_db_surveillance[i];
        }
    }

    return nullptr;
}

// Get database statistics
inline void get_db_stats(size_t& total, size_t& categories) {
    total = FREQ_DB_SIZE;
    categories = 11;  // Number of frequency categories
}

#endif /* __FREQ_DATABASE_H__ */
