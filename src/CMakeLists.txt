add_executable(multicast_rx multicast_rx.c)
target_compile_features(multicast_rx PRIVATE c_std_11)
add_executable(multicast_tx multicast_tx.c)
target_compile_features(multicast_tx PRIVATE c_std_11)

add_executable(unicast_rx unicast_rx.c)
target_compile_features(unicast_rx PRIVATE c_std_11)
add_executable(unicast_tx unicast_tx.c)
target_compile_features(unicast_tx PRIVATE c_std_11)

#----- linux only ----------
if(UNIX AND NOT (APPLE OR CYGWIN))
  add_executable(get_interfaces linux/get_interfaces.c)
  add_test(NAME InterfaceNames COMMAND get_interfaces)
  set_tests_properties(InterfaceNames PROPERTIES TIMEOUT 5)
endif()

#------------ HDF5  ----------
find_package(HDF5 COMPONENTS C)
include(CheckCSourceCompiles)
set(CMAKE_REQUIRED_INCLUDES ${HDF5_INCLUDE_DIRS})
set(CMAKE_REQUIRED_LIBRARIES ${HDF5_LIBRARIES})
check_c_source_compiles("#include \"hdf5.h\"
int main(void){printf(H5_VERS_MAJOR); return 0;}" HDF5_OK)

if(HDF5_OK)
  add_executable(unicast_rx_h5 hdf5/unicast_rx_h5.c)
  target_include_directories(unicast_rx_h5 PRIVATE ${HDF5_INCLUDE_DIRS})
  target_link_libraries(unicast_rx_h5 ${HDF5_C_LIBRARIES})
  #target_compile_options(unicast_rx_h5 PRIVATE -Wno-reserved-id-macro) #hdf5.h lint
endif()
