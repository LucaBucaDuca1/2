# RF Scanner Application for PortaPack Mayhem
# Add this to your PortaPack firmware build

project(rf_scanner)

set(MODE_CPPSRC
    rf_scanner.cpp
)

set(MODE_HEADERS
    rf_scanner.hpp
    freq_database.hpp
)

# This app should be added to the main firmware CMakeLists.txt
# in the applications directory
