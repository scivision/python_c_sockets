cmake_minimum_required(VERSION 3.25...4.2)

project(sockets LANGUAGES C CXX)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include(cmake/compilers.cmake)
include(cmake/kissnet.cmake)

set(WINSOCK_LIBRARIES "$<$<BOOL:${WIN32}>:wsock32;ws2_32;Iphlpapi>")

foreach(t multicast_rx multicast_tx udp_rx udp_tx)
  add_executable(${t} src/${t}.c)
  target_compile_features(${t} PRIVATE c_std_11)
  target_link_libraries (${t} PRIVATE ${WINSOCK_LIBRARIES})
  target_include_directories(${t} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
endforeach()

# --- kissnet
add_executable(udp_kiss_rx src/loopback_udp.cpp)
target_compile_features(udp_kiss_rx PRIVATE cxx_std_17)
target_include_directories(udp_kiss_rx PRIVATE ${kissnet_SOURCE_DIR})
target_link_libraries(udp_kiss_rx PRIVATE ${WINSOCK_LIBRARIES})

add_test(NAME LoopbackUDP COMMAND udp_kiss_rx)
set_tests_properties(LoopbackUDP PROPERTIES LABELS "kissnet")

# --- linux only
if(LINUX)
  add_executable(get_interfaces src/linux/get_interfaces.c)

  add_test(NAME InterfaceNames COMMAND get_interfaces)
  set_tests_properties(InterfaceNames PROPERTIES
  TIMEOUT 5
  LABELS "linux"
  )
endif()

add_subdirectory(src)
