cmake_minimum_required (VERSION 3.0)
project(spectrum C)

# cmake -DCMAKE_C_COMPILER=/usr/bin/clang  ..

add_compile_options(-mtune=native -ffast-math -Wall -Wextra -Wpedantic -fexceptions)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -std=c11)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_C_COMPILER_VERSION VERSION_LESS 3.6)
        message(FATAL_ERROR "Clang version must be at least 3.6!")
    endif()
    add_compile_options(-Weverything -Werror=array-bounds)
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_C_COMPILER_VERSION VERSION_LESS 5.2)
       message(FATAL_ERROR "GCC version must be at least 5.2!")
    endif()
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
    add_executable("${fname}" ${fn})
endforeach()

#FIXME cmake should be smart enough with find_library(HDF5) but not quite it seems...
# execute_process is a horrible hack
execute_process(COMMAND h5cc -o unicast_rx_h5 ../hdf5/unicast_rx_h5.c) 
#find_library(HDF5 REQUIRED)
#add_executable(unicast_rx_h5 hdf5/unicast_rx_h5.c)
#target_link_libraries(unicast_rx_h5 -lhdf5)

