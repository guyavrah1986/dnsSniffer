# CMakeLists.txt file for the utils library:
# ==========================================
set(libName "utilsLib")

add_library(${libName} SHARED utils.c hashTable.c)

# For test covrage - enable it back
#target_compile_options(${libName} PUBLIC -g -O0 -Wall -fprofile-arcs -ftest-coverage)
#target_link_options(${libName} PUBLIC -fprofile-arcs -ftest-coverage)

message(STATUS "CMakeLists.txt of ${libName} - created library")