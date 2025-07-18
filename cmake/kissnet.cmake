include(FetchContent)

set(kissnet_BUILD_TESTING off)

FetchContent_Declare(kissnet
  GIT_REPOSITORY https://github.com/Ybalrid/kissnet.git
  GIT_TAG d37730a8fbe71a3cb6537190fe3f53eea4070602
  GIT_SHALLOW true
)

FetchContent_MakeAvailable(kissnet)
