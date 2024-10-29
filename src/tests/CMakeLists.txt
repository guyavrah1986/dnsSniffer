# CMakeLists.txt for the unit tests:
# =================================
message(STATUS "CMakeLists.txt for unit tests of ${CMAKE_PROJECT_NAME}")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
find_library(GTEST_LIBRARY gtest HINTS /usr/lib)
message(STATUS "GTEST_INCLUDE_DIRS is: ${GTEST_INCLUDE_DIRS}")
message(STATUS "GTEST_LIBRARY is: ${GTEST_LIBRARY}")
message(STATUS "GTEST_LIBRARIES is: ${GTEST_LIBRARIES}")

set(testsExeName "unitTestsExe.out")
add_executable(${testsExeName} unitTestsMain.cpp)

# Due to the fact that the tcmalloc is installed into a "non standra" location
# adding here an "extra" location for the linker to look for libraries. Note
# that under these additional path(s) the linker ONLY looks for libraries 
# with names according to the convention, for example:libtcmalloc.so 
# If there is NOT a single file like so under the below path, the linker will
# report an error in this case.
#link_directories(/usr/lib/x86_64-linux-gnu)

# Add the gperftools include directory for the heap checker that will be 
# used in the unit tests.
# target_include_directories(${testsExeName} PUBLIC /usr/include/gperftools)

# NOTES:
# tcmalloc - is linked in order to use tcmalloc in the unit tests instead of glibc malloc - only
# for the sake of using the heap checker feature of the tcmalloc library.
# All other libraries are project's propriatry libraires that are to be tested.
target_link_libraries(${testsExeName} ${GTEST_LIBRARY} pthread parserLib utilsLib) #tcmalloc

# For test covrage:
message("BUILD_UNIT_TESTS=${BUILD_UNIT_TESTS}")
if( BUILD_UNIT_TESTS ) 
    message("BUILD_UNIT_TESTS evaluates to True")
    target_compile_options(${testsExeName} PUBLIC ${unit_tests_compile_options})
    target_link_options(${testsExeName} PUBLIC ${unit_tests_link_options})
    message(STATUS "setting additional compiler flags for unit testing:${unit_tests_compile_options}")
    message(STATUS "setting additional linker flags for unit testing:${unit_tests_link_options}")
endif()

# For debug
get_target_property(target_includes ${testsExeName} INTERFACE_INCLUDE_DIRECTORIES)
foreach(include_dir ${target_includes})
    message("Include directory: ${include_dir}")
endforeach()
