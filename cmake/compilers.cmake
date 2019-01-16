if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_compile_options(-g -O0)
else()
  add_compile_options(-O3)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL GNU)
  add_compile_options(-mtune=native -Wall -Wextra -Wpedantic -fexceptions)
endif() 
