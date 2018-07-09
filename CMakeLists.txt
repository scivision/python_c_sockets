cmake_minimum_required (VERSION 3.7)
project(sockets C)
enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_compile_options(-g -O0)
else()
  add_compile_options(-O3)
endif()

if(${CMAKE_C_COMPILER_ID} STREQUAL GNU)
  add_compile_options(-mtune=native -Wall -Wextra -Wpedantic -fexceptions)
endif() 
#-------- exe
add_executable(multicast_rx multicast_rx.c)

add_executable(multicast_tx multicast_tx.c)

add_executable(unicast_rx unicast_rx.c)

add_executable(unicast_tx unicast_tx.c)

#----- linux only ----------
if(UNIX AND NOT (APPLE OR CYGWIN))
    add_executable(get_interfaces linux/get_interfaces.c)
    add_test(NAME InterfaceNames COMMAND get_interfaces)
    set_tests_properties(InterfaceNames PROPERTIES TIMEOUT 5)
endif()

#------------ HDF5  ----------
# ftp://www.hdfgroup.org/HDF5/current/src/unpacked/release_docs/USING_HDF5_CMake.txt
find_package(HDF5)
if(HDF5_FOUND)
  add_executable(unicast_rx_h5 ../hdf5/unicast_rx_h5.c)
  target_include_directories(unicast_rx_h5 PRIVATE ${HDF5_INCLUDE_DIR})
  target_link_libraries(unicast_rx_h5 ${HDF5_C_LIBRARIES})
  #target_compile_options(unicast_rx_h5 PRIVATE -Wno-reserved-id-macro) #hdf5.h lint
endif()

