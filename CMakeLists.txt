cmake_minimum_required(VERSION 3.7)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Debug or Release")
endif()
project(sockets
LANGUAGES C)
enable_testing()

if(WIN32)
message(FATAL_ERROR "These programs don't have all the extra stuff needed for Windows. Consider Cygwin or WSL.")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(cmake/compilers.cmake)

add_subdirectory(src)