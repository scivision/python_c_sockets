include(FetchContent)

FetchContent_Declare(kissnet
  GIT_REPOSITORY https://github.com/Ybalrid/kissnet.git
  GIT_TAG master
  CMAKE_ARGS "-Dkissnet_BUILD_TESTING:BOOL=off"
)

FetchContent_MakeAvailable(kissnet)
