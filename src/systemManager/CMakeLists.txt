# CMakeLists.txt file for the system manager library:
# ===================================================
set(libName "systemManagerLib")

add_library(${libName} STATIC systemManager.c)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# For test covrage - enable it back
#target_compile_options(${libName} PUBLIC -g -O0 -Wall -fprofile-arcs -ftest-coverage)
#target_link_options(${libName} PUBLIC -fprofile-arcs -ftest-coverage)

# Link the pthread library
target_link_libraries(${libName} PRIVATE pthread)

target_link_libraries(${libName} PUBLIC listenerLib)

message(STATUS "CMakeLists.txt of ${libName} - created library")