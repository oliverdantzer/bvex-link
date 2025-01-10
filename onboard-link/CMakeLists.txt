cmake_minimum_required(VERSION 3.10)

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
find_package(nlohmann_json CONFIG REQUIRED)
find_package(nanopb CONFIG REQUIRED)

aux_source_directory(src _src)
aux_source_directory(src/encode_downlink_telemetry _encDownlink)
aux_source_directory(src/encode_downlink_telemetry/pb_generated _encDownlinkPbGen)
aux_source_directory(src/decode_onboard_telemetry _decOnboard)
aux_source_directory(src/decode_onboard_telemetry/pb_generated _decOnboardPbGen)
set(_srcFiles
    ${_src}
    ${_encDownlink}
    ${_encDownlink}
    ${_decOnboard}
    ${_decOnboardPbGen})

# exclude these files
list(REMOVE_ITEM _srcFiles
    "src/sample_transmitter.hpp"
    "src/telecommand.hpp"
    "src/telecommand.cpp"
    "src/chunker.hpp"
    "src/decode.hpp"
    "src/encode_telemetry.hpp"
)

message(STATUS "SOURCES: ${_srcFiles}")

add_executable(main ${_srcFiles})

target_link_libraries(main PRIVATE 
                    Boost::asio 
                    nlohmann_json::nlohmann_json
                    nanopb::protobuf-nanopb-static
                    )
