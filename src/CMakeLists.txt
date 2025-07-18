include(CheckSourceCompiles)

foreach(t multicast_rx multicast_tx udp_rx udp_tx)
  add_executable(${t} ${t}.c)
  target_compile_features(${t} PRIVATE c_std_11)
  target_link_libraries (${t} PRIVATE ${WINSOCK_LIBRARIES})
endforeach()

# --- kissnet
add_executable(udp_kiss_rx loopback_udp.cpp)
target_compile_features(udp_kiss_rx PRIVATE cxx_std_17)
target_include_directories(udp_kiss_rx PRIVATE ${kissnet_SOURCE_DIR})
target_link_libraries(udp_kiss_rx PRIVATE ${WINSOCK_LIBRARIES})
add_test(NAME LoopbackUDP COMMAND udp_kiss_rx)

# --- linux only
if(LINUX)
  add_executable(get_interfaces linux/get_interfaces.c)
  add_test(NAME InterfaceNames COMMAND get_interfaces)
  set_tests_properties(InterfaceNames PROPERTIES TIMEOUT 5)
endif()

# --- HDF5
find_package(HDF5 COMPONENTS C)
if(NOT HDF5_FOUND)
  return()
endif()

set(CMAKE_REQUIRED_LIBRARIES HDF5::HDF5)

check_source_compiles(C
[=[
#include "hdf5.h"
int main(void){
  printf("%d\n", H5_VERS_MAJOR);
  return 0;
}
]=]
HDF5_OK
)

if(HDF5_OK)
add_executable(udp_rx_h5 hdf5/udp_rx_h5.c)
target_include_directories(udp_rx_h5 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(udp_rx_h5 HDF5::HDF5 ${WINSOCK_LIBRARIES})
#target_compile_options(udp_rx_h5 PRIVATE -Wno-reserved-id-macro) #hdf5.h lint
endif()
