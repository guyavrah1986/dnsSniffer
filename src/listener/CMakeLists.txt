# CMakeLists.txt file for the listener library:
# ============================================
set(libName "listenerLib")

add_library(${libName} SHARED listener.c)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Find the libpcap library
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

# For test covrage - enable it back
#target_compile_options(${libName} PUBLIC -g -O0 -Wall -fprofile-arcs -ftest-coverage)
#target_link_options(${libName} PUBLIC -fprofile-arcs -ftest-coverage)

# Link the libpcap library to the target
target_link_libraries(${libName} PRIVATE ${PCAP_LIBRARIES})

message(STATUS "CMakeLists.txt of ${libName} - created library")