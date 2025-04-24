cmake_minimum_required(VERSION 3.24)

project(bcp-redis-client VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(nlohmann_json REQUIRED)

add_library(bcp-redis-client
    src/codec.cpp
    src/codec.c
    src/send_sample.c
)

target_link_libraries(bcp-redis-client PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(bcp-redis-client
    PUBLIC include
)

# ------------- START UNIT TESTS -------------
option(BCP_REDIS_CLIENT_TESTS "Build test executable with Google Test" OFF)

if(BCP_REDIS_CLIENT_TESTS)
    find_package(GTest REQUIRED)
    add_executable(gtest
        tests/gtest/codec.cpp
        tests/gtest/codec_c.cpp
    )
    set_target_properties(gtest PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(gtest PRIVATE 
        GTest::gtest
        GTest::gtest_main
        bcp-redis-client
        nlohmann_json::nlohmann_json
        # GTest::gmock
        # GTest::gmock_main
    )
    target_compile_definitions(gtest PRIVATE BCP_FETCH_BOUNDS_CHECKING)

    include(GoogleTest)
    gtest_discover_tests(gtest)
endif()

# ------------- END UNIT TESTS -------------

option(BCP_FETCH_TEST "Build test executables" OFF)

if(BCP_FETCH_TEST)
    message(STATUS "Test mode is ON")
    add_definitions(-DBCP_FETCH_TEST)
    
    target_compile_definitions(
        bcp-fetch PRIVATE DEBUG
    )

    set(REQUEST_SERVER_ADDR "\"localhost\"")
    set(REQUEST_SERVER_PORT "\"8080\"")
    set(SAMPLE_SERVER_ADDR "\"localhost\"")
    set(SAMPLE_SERVER_PORT "\"3000\"")

    # Common test settings macro
    macro(configure_test_executable target_name)
        target_include_directories(${target_name} 
            PRIVATE tests/common
            PRIVATE tests/${target_name}
        )
        target_link_libraries(${target_name} PRIVATE nanopb::protobuf-nanopb-static bcp-fetch m)
        target_compile_definitions(
            ${target_name} PRIVATE 
                REQUEST_SERVER_ADDR=${REQUEST_SERVER_ADDR}
                REQUEST_SERVER_PORT=${REQUEST_SERVER_PORT}
                SAMPLE_SERVER_ADDR=${SAMPLE_SERVER_ADDR}
                SAMPLE_SERVER_PORT=${SAMPLE_SERVER_PORT}
        )
    endmacro()

    # Test executable for request_sample
    add_executable(request_sample
        tests/request_sample/main.c
        tests/common/common.c
    )
    configure_test_executable(request_sample)

    # Test executable for send_sample
    add_executable(send_sample
        tests/send_sample/main.c
        tests/send_sample/test_loop.c
        tests/common/common.c
    )
    configure_test_executable(send_sample)

    # Test executable for command_server
    add_executable(command_server
        tests/command_server/main.c
        tests/common/common.c
    )
    configure_test_executable(command_server)

    # Test executable for command client
    add_executable(command_client
        tests/command_client/main.c
        tests/command_client/test_client.c
        tests/common/common.c
    )
    configure_test_executable(command_client)

endif()