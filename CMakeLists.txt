# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("MemPool")

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake
)

include(CTest)

# Include sub-projects.
add_subdirectory ("src")
add_subdirectory ("tests")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
   # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

#include(ConfigGBench)
#set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

include(ConfigGTest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
