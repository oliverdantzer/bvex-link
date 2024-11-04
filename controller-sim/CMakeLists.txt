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
include_directories(src)

# Collect source files
file(GLOB SOURCES "src/*.c" "src/*.h" 
    src/generated/nanopb/sample.pb.h
    src/generated/nanopb/sample.pb.c)

# Add the executable
add_executable(main ${SOURCES})

# Link libraries
target_link_libraries(main PRIVATE nanopb::protobuf-nanopb-static m)