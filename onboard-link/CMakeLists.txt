cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(onboard-link VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(Boost_DEBUG 1)
find_package(Boost 1.82.0) 

if(Boost_FOUND)
    add_executable(onboard-link test-asio.cpp) 
    # include_directories(${Boost_INCLUDE_DIR})
    target_link_libraries(onboard-link Boost)
endif()

# Define _WIN32_WINNT
target_compile_definitions(onboard-link PUBLIC _WIN32_WINNT=0x0A00)