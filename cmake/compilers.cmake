
if(CMAKE_C_COMPILER_ID STREQUAL GNU)
  add_compile_options(-mtune=native -Wall -Wextra -Wpedantic -fexceptions)
endif()
