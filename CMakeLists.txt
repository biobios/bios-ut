cmake_minimum_required(VERSION 3.14)
project(bios-ut CXX)

# Require C++17 or later for features like string literals, etc.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target
add_library(bios_ut src/printer.cpp src/runner.cpp)
target_include_directories(bios_ut PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Example target
add_subdirectory(example)
