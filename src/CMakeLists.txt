# CMakeLists.txt file for the main application (executable)
# =========================================================
set(exeName "dnsSniffer.out")
message(STATUS "CMakeLists.txt of ${exeName} executable")

# Find the libpcap library
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

# Add the following compilation settings (flags) globally to ALL targets
# in this project
add_compile_options(-Wall -Wextra -Wpedantic) #  -Werror

include_directories(src)

# Add the executable which is of now, composed ONLY from the main.c
add_executable(${exeName} main.c)

# Link the executable with all relevant libraries, be it propraeitry or 
# 3rd party
target_link_libraries(${exeName} PUBLIC parserLib utilsLib)

# Link the libpcap library to the target
target_link_libraries(${exeName} PRIVATE ${PCAP_LIBRARIES})

# Add the binary tree to the search path for include files
target_include_directories(${exeName} PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

