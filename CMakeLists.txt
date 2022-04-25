cmake_minimum_required(VERSION 3.19...3.23)

project(sockets
LANGUAGES C CXX)

include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include(cmake/compilers.cmake)
include(cmake/kissnet.cmake)

set(WINSOCK_LIBRARIES "$<$<BOOL:${WIN32}>:wsock32;ws2_32;Iphlpapi>")

add_subdirectory(src)
