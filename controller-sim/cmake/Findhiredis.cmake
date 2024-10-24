# From: https://github.com/hmartiro/redox/blob/master/cmake/Findhiredis.cmake
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Try to find hiredis
# Once done, this will define
#
# HIREDIS_FOUND        - system has hiredis
# HIREDIS_INCLUDE_DIRS - hiredis include directories
# HIREDIS_LIBRARIES    - libraries need to use hiredis

if(HIREDIS_INCLUDE_DIRS AND HIREDIS_LIBRARIES)
  set(HIREDIS_FIND_QUIETLY TRUE)
else()
  find_path(
    HIREDIS_INCLUDE_DIR
    NAMES hiredis/hiredis.h
    HINTS ${HIREDIS_ROOT_DIR}
    PATH_SUFFIXES include)

  find_library(
    HIREDIS_LIBRARY
    NAMES hiredis
    HINTS ${HIREDIS_ROOT_DIR}
    PATH_SUFFIXES ${CMAKE_INSTALL_LIBDIR})

  set(HIREDIS_INCLUDE_DIRS ${HIREDIS_INCLUDE_DIR})
  set(HIREDIS_LIBRARIES ${HIREDIS_LIBRARY})

  include (FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    hiredis DEFAULT_MSG HIREDIS_LIBRARY HIREDIS_INCLUDE_DIR)

  mark_as_advanced(HIREDIS_LIBRARY HIREDIS_INCLUDE_DIR)
endif()