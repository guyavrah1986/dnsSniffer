# CMakeLists.txt file for the DB (Data Base) manager library:
# ==========================================================
set(libName "dataBaseManagerLib")

add_library(${libName} SHARED dataBaseManager.c)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# For test covrage - enable it back
#target_compile_options(${libName} PUBLIC -g -O0 -Wall -fprofile-arcs -ftest-coverage)
#target_link_options(${libName} PUBLIC -fprofile-arcs -ftest-coverage)

message(STATUS "CMakeLists.txt of ${libName} - created library")