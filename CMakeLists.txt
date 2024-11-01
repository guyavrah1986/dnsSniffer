# This is the project's main CMakeLists.txt
#========================================== 
cmake_minimum_required(VERSION 3.16)
project(dnsSniffer)

message(STATUS "main CMakeLists.txt of ${CMAKE_PROJECT_NAME}")
message(STATUS "The root (top level) folder of the project is:${PROJECT_SOURCE_DIR}")

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/parser)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/utils)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/tests)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)