cmake_minimum_required(VERSION 3.24)

project(controller-sim VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

find_package(nanopb CONFIG REQUIRED)

option(TEST_SOCK_PORT "Enable printing socket port number" OFF)
IF(TEST_SOCK_PORT)
    message(STATUS "Socket port number will be printed")
    add_definitions(-DTEST_SOCK_PORT)
ENDIF()

add_library(telemetry-uplink
    src/generated/nanopb/sample.pb.c
    src/generated/nanopb/primitive.pb.c
    src/generated/nanopb/request.pb.c
    src/generated/nanopb/response.pb.c
    src/request.c
    src/socket.c
    src/send_telemetry.c
)

target_link_libraries(telemetry-uplink PRIVATE nanopb::protobuf-nanopb-static)
target_include_directories(telemetry-uplink
    PRIVATE src/generated/nanopb
    PUBLIC include
)

option(TEST "Build test executable" OFF)

if(TEST)
    message(STATUS "Test mode is ON")
    add_definitions(-DTEST)
    
    target_compile_definitions(
        telemetry-uplink PRIVATE DEBUG
    )

    # Collect source files for the test executable
    file(GLOB TEST_SOURCES 
        "tests/*.c"
        )

    # Create the test executable target
    add_executable(test ${TEST_SOURCES})

    target_include_directories(test PRIVATE tests)

    # Link telemetry-uplink
    target_link_libraries(test PRIVATE nanopb::protobuf-nanopb-static telemetry-uplink m)

    set(REQUEST_SERVER_ADDR "\"localhost\"")
    set(REQUEST_SERVER_PORT "\"8080\"")
    set(SAMPLE_SERVER_ADDR "\"localhost\"")
    set(SAMPLE_SERVER_PORT "\"3000\"")
    target_compile_definitions(
        test PRIVATE 
            REQUEST_SERVER_ADDR=${REQUEST_SERVER_ADDR}
            REQUEST_SERVER_PORT=${REQUEST_SERVER_PORT}
            SAMPLE_SERVER_ADDR=${SAMPLE_SERVER_ADDR}
            SAMPLE_SERVER_PORT=${SAMPLE_SERVER_PORT}
    )

endif()