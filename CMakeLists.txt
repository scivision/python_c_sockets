cmake_minimum_required(VERSION 3.14...3.20)

project(sockets
LANGUAGES C CXX)

include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include(cmake/compilers.cmake)
include(cmake/kissnet.cmake)

set(WINSOCK_LIBRARIES)
if(WIN32)
set(WINSOCK_LIBRARIES wsock32 ws2_32 Iphlpapi)
endif()

add_subdirectory(src)
