cmake_minimum_required (VERSION 3.1)
project(sockets C)

add_compile_options(-mtune=native -ffast-math -Wall -Wextra -Wpedantic -fexceptions)

add_executable(unicast_rx_h5 ../hdf5/unicast_rx_h5.c)

if (${CMAKE_C_COMPILER_ID} STREQUAL Clang)
    add_compile_options(-Weverything -Werror=array-bounds) 
    target_compile_options(unicast_rx_h5 PRIVATE -Wno-reserved-id-macro) #hdf5.h lint
elseif(${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    add_compile_options(-Warray-bounds=2)
endif()

set(CMAKE_VERBOSE_MAKEFILE OFF) # see what the individual compiler commands are
# ------------ Build mode selections ---------------------------------
set(CMAKE_BUILD_TYPE Release) #invokes (some) compiler optimizations
#set(CMAKE_BUILD_TYPE Debug) # no optimizations, (some) debug flags on, injects debugging symbols

#-------------
file(GLOB flist *.c)
foreach(fn ${flist})
    get_filename_component(fname ${fn} NAME_WE)
    add_executable(${fname} ${fn})
    set_property(TARGET ${fname} PROPERTY C_STANDARD 11)
endforeach()
#------------ HDF5  tested with CMake 3.5, HDF5 1.8.16 ----------
# ftp://www.hdfgroup.org/HDF5/current/src/unpacked/release_docs/USING_HDF5_CMake.txt
find_package(HDF5 REQUIRED)
INCLUDE_DIRECTORIES (${HDF5_INCLUDE_DIR})
target_link_libraries(unicast_rx_h5 ${HDF5_C_LIBRARIES})

