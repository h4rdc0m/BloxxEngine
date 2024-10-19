file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_library(BloxxEngine ${ENGINE_SOURCES})

target_link_libraries(BloxxEngine SDL3::SDL3)

target_include_directories(BloxxEngine PUBLIC include)
