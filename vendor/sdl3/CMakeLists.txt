# https://github.com/libsdl-org/SDL
# https://wiki.libsdl.org/SDL3/README-cmake

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TESTS OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_EXAMPLES OFF)

#include(FetchContent)
#FetchContent_Declare(
#  SDL3
#  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#  GIT_TAG edfbfa2
#)
#FetchContent_MakeAvailable(SDL3)

add_subdirectory(code)
