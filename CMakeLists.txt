cmake_minimum_required(VERSION 3.0)
project(dnsSniffer)

# Find the libpcap library
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

# Define the executable target
add_executable(dnsSniffer.out main.c)

# Link the libpcap library to the target
target_link_libraries(dnsSniffer.out PRIVATE ${PCAP_LIBRARIES})

# Include the libpcap headers
target_include_directories(dnsSniffer.out PRIVATE ${PCAP_INCLUDE_DIRS})
