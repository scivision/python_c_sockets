cmake_minimum_required(VERSION 3.25...4.2)

project(sockets LANGUAGES C CXX)

enable_testing()

include(CheckSymbolExists)

include(cmake/compilers.cmake)
include(cmake/kissnet.cmake)

set(WINSOCK_LIBRARIES "$<$<BOOL:${WIN32}>:wsock32;ws2_32;Iphlpapi>")

foreach(t multicast_rx multicast_tx udp_rx udp_tx)
  add_executable(${t} src/${t}.c)
  target_compile_features(${t} PRIVATE c_std_11)
  target_link_libraries (${t} PRIVATE ${WINSOCK_LIBRARIES})
  target_include_directories(${t} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
endforeach()

find_package(Python COMPONENTS Interpreter)

if(Python_Interpreter_FOUND)
  add_test(NAME Unicast
  COMMAND Python::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/test/simul_exe.py $<TARGET_FILE:udp_tx> $<TARGET_FILE:udp_rx>)

  add_test(NAME Multicast
  COMMAND Python::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/test/simul_exe.py $<TARGET_FILE:multicast_tx> $<TARGET_FILE:multicast_rx>)

# CMake / CTest cmd /c has long-standing known issues cascading cmd /c with
# subsequent commands like start /b. The easy and robust alternative is using
# Python subprocess.Popen() to run the executables in parallel.

  set_tests_properties(Multicast Unicast PROPERTIES TIMEOUT 10 LABELS "multicast")
endif()

# --- kissnet
add_executable(udp_kiss_rx src/loopback_udp.cpp)
target_compile_features(udp_kiss_rx PRIVATE cxx_std_17)
target_include_directories(udp_kiss_rx PRIVATE ${kissnet_SOURCE_DIR})
target_link_libraries(udp_kiss_rx PRIVATE ${WINSOCK_LIBRARIES})

target_compile_definitions(multicast_tx PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

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

# --- HDF5
find_package(HDF5 COMPONENTS C)
if(HDF5_FOUND)
  block()
  set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
  set(CMAKE_REQUIRED_LIBRARIES HDF5::HDF5)

  check_symbol_exists(H5_VERS_MAJOR "hdf5.h" HDF5_OK)
  endblock()

  if(HDF5_OK)
    add_executable(udp_rx_h5 src/udp_rx_h5.c)
    target_include_directories(udp_rx_h5 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(udp_rx_h5 HDF5::HDF5 ${WINSOCK_LIBRARIES})
  endif()
endif()

file(GENERATE OUTPUT .gitignore CONTENT "*")
