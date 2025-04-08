cmake_minimum_required(VERSION 3.24)

project(controller-sim
    VERSION 0.0.1
    DESCRIPTION "Provides a C library for sending and requesting data. Intended to be consumed by BVEX's onboard computer code."
    HOMEPAGE_URL "https://github.com/oliverdantzer/bvex-link/tree/main/bcp-fetch-client"
    LANGUAGES C CXX)

# Build options
option(BCP_FETCH_TEST "Build test executables" OFF)
option(BCP_FETCH_GTEST "Build test executable with Google Test" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings using cython" OFF)
find_package(nanopb CONFIG REQUIRED)

# Compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

################################################################################
# bcp-fetch libraries
################################################################################

# Native C implementation ------------------------------------------------------
add_library(bcp-fetch
    src/generated/nanopb/sample.pb.c
    src/generated/nanopb/primitive.pb.c
    src/generated/nanopb/request.pb.c
    src/generated/nanopb/response.pb.c
    src/request_sample.c
    src/socket.c
    src/send_sample/sample_sender_utils.c
    src/send_sample/primitive_sender.c
    src/send_sample/file_sender.c
    src/command_server.c
)

target_link_libraries(bcp-fetch PRIVATE nanopb::protobuf-nanopb-static)
target_include_directories(bcp-fetch
    PRIVATE
        src/generated/nanopb
        src/send_sample
    PUBLIC include
)

# Python bindings --------------------------------------------------------------
if(BUILD_PYTHON_BINDINGS)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)

  # Prepare Python's setup.cfg
  file(GENERATE
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings/setup.cfg
    CONTENT
"
[build_ext]
library_dirs=$<TARGET_FILE_DIR:bcp-fetch>
include_dirs=$<JOIN:$<TARGET_PROPERTY:bcp-fetch,INCLUDE_DIRECTORIES>,:>
")

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python.stamp
    COMMAND ${Python3_EXECUTABLE} setup.py build_ext
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/python.stamp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings/setup.py
      ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings/setup.cfg
      ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings/src/bcp_fetch/__init__.py
      ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings/src/bcp_fetch/c_sample_sender.pxd
      ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings/src/bcp_fetch/sample_sender_ext.pxd
      ${CMAKE_CURRENT_SOURCE_DIR}/python-bindings/src/bcp_fetch/sample_sender_ext.pyx
      bcp-fetch
    COMMENT "Building Python bindings to 'bcp-fetch' library"
  )
  add_custom_target(bcp-fetch-python-bindings ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/python.stamp)
endif()

################################################################################
# Test executables
################################################################################

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

################################################################################
# Installation
################################################################################
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Config file to be findable by cmake
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/bcp-fetch-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/bcp-fetch-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/bcp-fetch)

# Version file for semantic version checks
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/bcp-fetch-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

# Native foo library
install(
  TARGETS bcp-fetch
  EXPORT bcp-fetch-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
  FILES include/bcp-fetch.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# CMake files for find_package()
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/bcp-fetch-config-version.cmake
    ${PROJECT_BINARY_DIR}/bcp-fetch-config.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/bcp-fetch)

# CMake targets for stuff like target_link_libraries()
install(
  EXPORT bcp-fetch-targets
  FILE bcp-fetch-targets.cmake
  NAMESPACE oliverdantzer::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/bcp-fetch)
export(
  EXPORT bcp-fetch-targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/bcp-fetch-targets.cmake
  NAMESPACE oliverdantzer::)
export(PACKAGE bcp-fetch)


# -----------------manual testing

# if(BCP_FETCH_TEST)
#     message(STATUS "Test mode is ON")
#     add_definitions(-DBCP_FETCH_TEST)
    
#     target_compile_definitions(
#         bcp-fetch PRIVATE DEBUG
#     )

#     set(REQUEST_SERVER_ADDR "\"localhost\"")
#     set(REQUEST_SERVER_PORT "\"8080\"")
#     set(SAMPLE_SERVER_ADDR "\"localhost\"")
#     set(SAMPLE_SERVER_PORT "\"3000\"")

#     # Common test settings macro
#     macro(configure_test_executable target_name)
#         target_include_directories(${target_name} 
#             PRIVATE tests/common
#             PRIVATE tests/manual/${target_name}
#         )
#         target_link_libraries(${target_name} PRIVATE nanopb::protobuf-nanopb-static bcp-fetch m)
#         target_compile_definitions(
#             ${target_name} PRIVATE 
#                 REQUEST_SERVER_ADDR=${REQUEST_SERVER_ADDR}
#                 REQUEST_SERVER_PORT=${REQUEST_SERVER_PORT}
#                 SAMPLE_SERVER_ADDR=${SAMPLE_SERVER_ADDR}
#                 SAMPLE_SERVER_PORT=${SAMPLE_SERVER_PORT}
#         )
#     endmacro()

#     # Test executable for request_sample
#     add_executable(request_sample
#         tests/manual/request_sample/main.c
#     )
#     configure_test_executable(request_sample)

#     # Test executable for send_sample
#     add_executable(send_sample
#         tests/manual/send_sample/main.c
#         tests/manual/send_sample/test_loop.c
#     )
#     configure_test_executable(send_sample)

#     # Test executable for command_server
#     add_executable(command_server
#         tests/manual/command_server/main.c
#     )
#     configure_test_executable(command_server)

#     # Test executable for command client
#     add_executable(command_client
#         tests/manual/command_client/main.c
#         tests/manual/command_client/test_client.c
#     )
#     configure_test_executable(command_client)

# endif()