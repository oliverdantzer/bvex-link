cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(onboard-link VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(DEBUG "Use verbose debug logging" OFF)

if(DEBUG)
    message(STATUS "Debug mode is ON")
    add_definitions(-DDEBUG)
endif()

find_package(Boost REQUIRED COMPONENTS asio)
find_package(nlohmann_json CONFIG REQUIRED)

if(Boost_FOUND)
    add_executable(main src/main.cpp src/send_server.cpp src/send_server.hpp src/recv_server.cpp src/recv_server.hpp)
    target_link_libraries(main PRIVATE Boost::asio)
    target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
endif()
