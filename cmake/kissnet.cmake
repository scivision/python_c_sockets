include(FetchContent)

# set(build_tests OFF CACHE INTERNAL "kissnet tests")
# necessary because kissnet has option(... ON)

FetchContent_Declare(kissnet
  # GIT_REPOSITORY https://github.com/Ybalrid/kissnet.git
  GIT_REPOSITORY https://github.com/scivision/kissnet.git
  GIT_TAG master
  CMAKE_ARGS "-Dbuild_tests:BOOL=off"
)

FetchContent_MakeAvailable(kissnet)
