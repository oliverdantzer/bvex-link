cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(controller-sim VERSION 1.0 LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the custom module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# # Find the hiredis package
# find_package(hiredis REQUIRED)

# Find nanopb
find_package(nanopb CONFIG REQUIRED)

# Include directories
include_directories(include src ${nanopb_INCLUDE_DIRS})

# Collect source files for the library
file(GLOB LIB_SOURCES 
    "src/*.c"
    "include/*.h"
    src/generated/nanopb/sample.pb.h
    src/generated/nanopb/sample.pb.c)

# Collect source files for the test executable
file(GLOB TEST_SOURCES 
    "tests/*.c"
    "tests/*.h"
    )

# Create the library target
add_library(telemetry-uplink ${LIB_SOURCES})

# Link nanopb to the library
target_link_libraries(telemetry-uplink PRIVATE nanopb::protobuf-nanopb-static)

# Create the test executable target
add_executable(test ${TEST_SOURCES})

# Link libraries
target_link_libraries(test PRIVATE telemetry-uplink nanopb::protobuf-nanopb-static m)
