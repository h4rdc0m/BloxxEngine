file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp src/*.h)
file(GLOB_RECURSE ENGINE_HEADERS include/*.h)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_library(BloxxEngine ${ENGINE_HEADERS} ${ENGINE_SOURCES})

find_package(Vulkan REQUIRED)
target_link_libraries(BloxxEngine glfw ImGui spdlog opengl32 glad glm stb event)

target_include_directories(BloxxEngine PUBLIC include)
