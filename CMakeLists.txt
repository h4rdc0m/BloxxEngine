cmake_minimum_required(VERSION 3.30)

project(BloxxEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Fetch the dependencies
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps")

# GLFW


set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

# Add the subdirectories for the engine and sandbox
add_subdirectory(BloxxEngine)
add_subdirectory(Sandbox)

