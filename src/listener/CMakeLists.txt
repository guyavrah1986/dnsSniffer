# CMakeLists.txt file for the listener library:
# ============================================
set(libName "listenerLib")

# GuyA: The requierment was to NOT assume that libpcap.so will be present on
# the target machine, so we need to create this library as a static one, so
# that it can be linked statically to libpcap.a (/usr/lib/x86_64-linux-gnu/libpcap.a)
add_library(${libName} SHARED listener.c)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Find the libpcap library
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

# For test covrage - enable it back
#target_compile_options(${libName} PUBLIC -g -O0 -Wall -fprofile-arcs -ftest-coverage)
#target_link_options(${libName} PUBLIC -fprofile-arcs -ftest-coverage)



# Locate the static libpcap library
#find_library(PCAP_STATIC_LIB NAMES pcap.a libpcap.a PATHS /usr/lib /usr/local/lib)
#message(STATUS "CMakeLists.txt of ${libName} - PCAP_STATIC_LIB is:${PCAP_STATIC_LIB}")

#if (NOT PCAP_STATIC_LIB)
#    message(FATAL_ERROR "Static libpcap library not found.")
#endif()



# Link the libpcap library to the target
target_link_libraries(${libName} PRIVATE ${PCAP_LIBRARIES})

target_link_libraries(${libName} PUBLIC parserLib)

message(STATUS "CMakeLists.txt of ${libName} - created library")