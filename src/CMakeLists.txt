include(CheckSourceCompiles)

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
