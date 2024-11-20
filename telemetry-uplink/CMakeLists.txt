cmake_minimum_required(VERSION 3.24)

# Set the project name and version
project(controller-sim VERSION 1.0 LANGUAGES C)

# Specify the C standard
# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED True)

# Find nanopb
find_package(nanopb CONFIG REQUIRED)

# Collect source files for the library
file(GLOB LIB_SOURCES 
    "src/*.c"
    src/generated/nanopb/sample.pb.c
    src/generated/nanopb/sample.pb.h
    )

add_library(telemetry-uplink ${LIB_SOURCES})

target_link_libraries(telemetry-uplink PRIVATE nanopb::protobuf-nanopb-static)
target_include_directories(telemetry-uplink
    PUBLIC include
)

option(TEST "Build test executable" OFF)

if(TEST)
    message(STATUS "Test mode is ON")
    add_definitions(-DTEST)

    # Collect source files for the test executable
    file(GLOB TEST_SOURCES 
        "tests/*.c"
        )

    # Create the test executable target
    add_executable(test ${TEST_SOURCES})

    target_include_directories(test PRIVATE tests)

    # Link telemetry-uplink
    target_link_libraries(test PRIVATE telemetry-uplink m)
endif()