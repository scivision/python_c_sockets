cmake_minimum_required (VERSION 2.8.12)
project(sockets C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-mtune=native -Wall -Wextra -Wpedantic -fexceptions)

file(GLOB flist *.c)
foreach(fn ${flist})
    get_filename_component(fname ${fn} NAME_WE)
    add_executable(${fname} ${fn})
endforeach()
#----- linux only ----------
if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    add_executable(get_interfaces linux/get_interfaces.c)
endif()
#------------ HDF5  ----------
# ftp://www.hdfgroup.org/HDF5/current/src/unpacked/release_docs/USING_HDF5_CMake.txt
find_package(HDF5)
if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIR})

    add_executable(unicast_rx_h5 ../hdf5/unicast_rx_h5.c)
    target_link_libraries(unicast_rx_h5 ${HDF5_C_LIBRARIES})
    #target_compile_options(unicast_rx_h5 PRIVATE -Wno-reserved-id-macro) #hdf5.h lint
endif()

