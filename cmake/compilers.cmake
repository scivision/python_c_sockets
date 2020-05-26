
if(CMAKE_C_COMPILER_ID STREQUAL GNU)
  add_compile_options(-mtune=native -Wall -Wextra)
endif()

# this helps linters e.g. Visual Studio Intellicode work properly
set(CMAKE_EXPORT_COMPILE_COMMANDS on)