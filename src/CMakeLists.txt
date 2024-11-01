# CMakeLists.txt file for the main application (executable)
# =========================================================
set(exeName "dnsSniffer.out")
message(STATUS "CMakeLists.txt of ${exeName} executable")

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

include_directories(src)

# Add the executable which is of now, composed ONLY from the main.c
add_executable(${exeName} main.c)

# Link the executable with all relevant libraries, be it propraeitry or 
# 3rd party
target_link_libraries(${exeName} PUBLIC parserLib utilsLib)

# Add the binary tree to the search path for include files
target_include_directories(${exeName} PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )

