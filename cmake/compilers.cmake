
if(CMAKE_C_COMPILER_ID STREQUAL GNU)
  add_compile_options("$<$<CONFIG:Debug,RelWithDebInfo>:-Wall;-Wextra>")
endif()
