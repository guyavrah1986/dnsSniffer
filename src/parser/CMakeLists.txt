# CMakeLists.txt file for the parser library:
# ===========================================
set(libName "parserLib")

add_library(${libName} SHARED parser.c)

# For test covrage - enable it back
#target_compile_options(${libName} PUBLIC -g -O0 -Wall -fprofile-arcs -ftest-coverage)
#target_link_options(${libName} PUBLIC -fprofile-arcs -ftest-coverage)

# Link the libpcap library to the target
target_link_libraries(${libName} PRIVATE ${PCAP_LIBRARIES})

# Include the libpcap headers
target_include_directories(${libName} PRIVATE ${PCAP_INCLUDE_DIRS})

message(STATUS "CMakeLists.txt of ${libName} - created library")