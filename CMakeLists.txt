cmake_minimum_required(VERSION 3.14)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug or Release")
endif()
project(sockets
LANGUAGES C CXX)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include(cmake/compilers.cmake)
include(cmake/kissnet.cmake)

set(WINSOCK_LIBRARIES)
if(WIN32)
set(WINSOCK_LIBRARIES wsock32 ws2_32 Iphlpapi)
endif()

add_subdirectory(src)
