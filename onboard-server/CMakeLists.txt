cmake_minimum_required(VERSION 3.26)

# set the project name and version
project(onboard-link VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(DEBUG "Use verbose debug logging" ON)

if(DEBUG)
    message(STATUS "Debug mode is ON")
    add_definitions(-DDEBUG)
endif()

find_package(boost_asio CONFIG REQUIRED)
find_package(nanopb CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
    src/command.cpp
    src/decode_onboard_telemetry/decode_sample_pb.cpp
    src/decode_onboard_telemetry/pb_generated/sample.pb.c
    src/encode_downlink_telemetry/encode_file.cpp
    src/encode_downlink_telemetry/encode_primitive.cpp
    src/encode_downlink_telemetry/encode_sample_frame.cpp
    src/encode_downlink_telemetry/pb_generated/file.pb.c
    src/encode_downlink_telemetry/pb_generated/primitive.pb.c
    src/encode_downlink_telemetry/pb_generated/sample.pb.c
    src/encode_downlink_telemetry/serialize_nanopb_struct.cpp
    src/sample.cpp
    src/server/recv_server.cpp
    src/server/recv_uplink_sample_server.cpp
    src/server/send_server.cpp
    src/telemetry.cpp
    src/utils/chunker.cpp
    src/utils/sample_transmitter.cpp
    src/utils/telecommand.cpp
)

add_executable(main ${SOURCES} ${HEADERS})

target_include_directories(main PRIVATE
    src
    src/decode_onboard_telemetry
    src/decode_onboard_telemetry/pb_generated
    src/encode_downlink_telemetry
    src/encode_downlink_telemetry/pb_generated
    src/server
    src/utils
)

target_link_libraries(main PRIVATE 
    Boost::asio 
    nanopb::protobuf-nanopb-static
    )
