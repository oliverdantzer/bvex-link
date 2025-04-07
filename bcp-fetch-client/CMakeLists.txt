cmake_minimum_required(VERSION 3.24)

project(controller-sim VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(nanopb CONFIG REQUIRED)

add_library(bcp-fetch
    src/generated/nanopb/sample.pb.c
    src/generated/nanopb/primitive.pb.c
    src/generated/nanopb/request.pb.c
    src/generated/nanopb/response.pb.c
    src/request_sample.c
    src/socket.c
    src/send_sample/sample_sender.c
    src/send_sample/primitive_sender.c
    src/send_sample/file_sender.c
    src/command_server.c
)

target_link_libraries(bcp-fetch PRIVATE nanopb::protobuf-nanopb-static)
target_include_directories(bcp-fetch
    PRIVATE
        src/generated/nanopb
    PUBLIC include
)

# ------------- START UNIT TESTS -------------
option(BCP_FETCH_GTEST "Build test executable with Google Test" OFF)

if(BCP_FETCH_GTEST)
    find_package(GTest REQUIRED)
    add_executable(gtest
        tests/gtest/connected_udp_socket.cpp
        tests/gtest/send_sample.cpp
        tests/gtest/request_sample.cpp
        tests/gtest/command_server.cpp
    )
    target_include_directories(gtest
        PRIVATE
            src/generated/nanopb
            tests/common
    )
    set_target_properties(gtest PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(gtest PRIVATE 
        GTest::gtest
        GTest::gtest_main
        bcp-fetch
        nanopb::protobuf-nanopb-static
    )

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
            PRIVATE tests/manual/${target_name}
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
        tests/manual/request_sample/main.c
    )
    configure_test_executable(request_sample)

    # Test executable for send_sample
    add_executable(send_sample
        tests/manual/send_sample/main.c
        tests/manual/send_sample/test_loop.c
    )
    configure_test_executable(send_sample)

    # Test executable for command_server
    add_executable(command_server
        tests/manual/command_server/main.c
    )
    configure_test_executable(command_server)

    # Test executable for command client
    add_executable(command_client
        tests/manual/command_client/main.c
        tests/manual/command_client/test_client.c
    )
    configure_test_executable(command_client)

endif()