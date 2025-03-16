cmake_minimum_required(VERSION 3.24)

project(controller-sim VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

find_package(nanopb CONFIG REQUIRED)

option(BCP_FETCH_BOUNDS_CHECKING "Explicitly check bounds on library function args" OFF)
IF(BCP_FETCH_BOUNDS_CHECKING)
    add_definitions(-BCP_FETCH_BOUNDS_CHECKING)
ENDIF()

option(TEST_SOCK_PORT "Enable printing socket port number" OFF)
IF(TEST_SOCK_PORT)
    add_definitions(-DTEST_SOCK_PORT)
ENDIF()

add_library(bcp-fetch
    src/generated/nanopb/sample.pb.c
    src/generated/nanopb/primitive.pb.c
    src/generated/nanopb/request.pb.c
    src/generated/nanopb/response.pb.c
    src/request_sample.c
    src/socket.c
    src/send_sample.c
)

target_link_libraries(bcp-fetch PRIVATE nanopb::protobuf-nanopb-static)
target_include_directories(bcp-fetch
    PRIVATE src/generated/nanopb
    PUBLIC include
)

option(BCP_FETCH_TEST "Build test executable" OFF)

if(BCP_FETCH_TEST)
    message(STATUS "Test mode is ON")
    add_definitions(-DBCP_FETCH_TEST)
    
    target_compile_definitions(
        bcp-fetch PRIVATE DEBUG
    )

    # Collect source files for the test executable
    file(GLOB TEST_SOURCES 
        "tests/*.c"
        )

    # Create the test executable target
    add_executable(test ${TEST_SOURCES})

    target_include_directories(test PRIVATE tests)

    # Link telemetry-uplink
    target_link_libraries(test PRIVATE nanopb::protobuf-nanopb-static bcp-fetch m)

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