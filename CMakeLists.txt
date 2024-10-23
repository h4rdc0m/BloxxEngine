cmake_minimum_required(VERSION 3.29)
project(Bloxx)

set(CMAKE_CXX_STANDARD 23)

# Fetch all external dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)

# ImGui (with docking branch)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.4-docking
)

# spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
)


# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)


# stb
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)

FetchContent_MakeAvailable(glfw imgui spdlog glm stb)

if(NOT TARGET stb)
    file(GLOB STB_SOURCES "${stb_SOURCE_DIR}/*.h")
    add_library(stb INTERFACE ${STB_SOURCES})

    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif()


# libevent
set(EVENT__DISABLE_OPENSSL ON)
FetchContent_Declare(
        libevent
        GIT_REPOSITORY https://github.com/libevent/libevent.git
        GIT_TAG release-2.1.12-stable
)

FetchContent_MakeAvailable(libevent)

if (NOT TARGET event)

    add_subdirectory(${libevent_SOURCE_DIR} ${libevent_BINARY_DIR})
endif ()

if (NOT TARGET imgui)
    # Include imgui files for the target
    set(IMGUI_SOURCES
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    add_library(ImGui STATIC ${IMGUI_SOURCES})
    target_include_directories(ImGui PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends)

    target_link_libraries(ImGui opengl32 glfw)
endif ()

if (NOT TARGET spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif ()

add_subdirectory(vendor/glad)
add_subdirectory(BloxxEngine)
add_subdirectory(Sandbox)