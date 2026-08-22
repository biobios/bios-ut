cmake_minimum_required(VERSION 3.14)
project(bios-ut CXX)

# Require C++17 or later for features like string literals, etc.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target
add_library(bios_ut STATIC src/printer.cpp src/runner.cpp)

# Modern CMake: Specify C++ standard requirement on the target itself
target_compile_features(bios_ut PUBLIC cxx_std_20)

target_include_directories(bios_ut PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Provide an ALIAS target (standard practice for modern CMake)
add_library(bios::ut ALIAS bios_ut)

# Main target
add_library(bios_ut_main STATIC src/default_main.cpp)
target_link_libraries(bios_ut_main PUBLIC bios_ut)
add_library(bios::ut_main ALIAS bios_ut_main)

# Example target: Only build if this is the main project (not when added via add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(example)
endif()
