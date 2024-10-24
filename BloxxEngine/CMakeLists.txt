file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp src/*.h)
file(GLOB_RECURSE ENGINE_HEADERS include/*.h)

add_library(BloxxEngine ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(BloxxEngine stdc++ glfw ImGui spdlog glad glm stb)

target_include_directories(BloxxEngine PUBLIC include)


set_target_properties(glfw PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(ImGui PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(spdlog PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(glad PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(glm PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(stb PROPERTIES EXCLUDE_FROM_ALL 1)