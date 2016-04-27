cmake_minimum_required (VERSION 2.8)
project(sockets C)

file(GLOB flist *.c)
foreach(fn ${flist})
    get_filename_component(fname ${fn} NAME_WE)
    add_executable(${fname} ${fn})
    set_target_properties(${fname} PROPERTIES C_STANDARD 11)
endforeach()
#----- linux only ----------
if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    add_executable(get_interfaces linux/get_interfaces.c)
endif()
#------------ HDF5  ----------
# ftp://www.hdfgroup.org/HDF5/current/src/unpacked/release_docs/USING_HDF5_CMake.txt
find_package(HDF5)
if(HDF5_FOUND)
    INCLUDE_DIRECTORIES (${HDF5_INCLUDE_DIR})

    add_executable(unicast_rx_h5 ../hdf5/unicast_rx_h5.c)
    target_link_libraries(unicast_rx_h5 ${HDF5_C_LIBRARIES})
    set_target_properties(unicast_rx_h5 PROPERTIES C_STANDARD 11)
    target_compile_options(unicast_rx_h5 PRIVATE -Wno-reserved-id-macro) #hdf5.h lint
endif()
#--------------
add_compile_options(-mtune=native -ffast-math -Wall -Wextra -Wpedantic -fexceptions)
if (${CMAKE_C_COMPILER_ID} STREQUAL Clang)
    add_compile_options(-Weverything -Werror=array-bounds) 
elseif(${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    add_compile_options(-Warray-bounds=2)
endif()

