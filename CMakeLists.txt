cmake_minimum_required(VERSION 3.29)
project(Bloxx)

set(CMAKE_CXX_STANDARD 23)

# Fetch all external dependencies
include(FetchContent)

# SDL3

# SDL3
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG preview-3.1.3
)
FetchContent_MakeAvailable(SDL3)

add_subdirectory(BloxxEngine)
add_subdirectory(Sandbox)