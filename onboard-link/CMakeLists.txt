cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(onboard-link VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED COMPONENTS asio)

if(Boost_FOUND)
    add_executable(onboard-link daytime-async-tcp-server.cpp)
    target_link_libraries(onboard-link PRIVATE Boost::asio)
endif()
