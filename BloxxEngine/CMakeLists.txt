file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp src/*.h)
file(GLOB_RECURSE ENGINE_HEADERS include/*.h)

add_library(BloxxEngine ${ENGINE_HEADERS} ${ENGINE_SOURCES})

# Platform check routine
message(STATUS "Checking platform...")

if (WIN32)
    message(STATUS "Detected platform: Windows")
    # Windows-specific settings
    set(PLATFORM_NAME "Windows")
    # Add more Windows-specific configurations or libraries here

elseif (APPLE)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-x objective-c++" HAS_OBJCXX)
    if (HAS_OBJCXX)
        message(STATUS "Detected platform: iOS")
        # iOS-specific settings
        set(PLATFORM_NAME "iOS")
        # Add more iOS-specific configurations or libraries here
    else()
        message(STATUS "Detected platform: macOS")
        # macOS-specific settings
        set(PLATFORM_NAME "macOS")
        # Add more macOS-specific configurations or libraries here
    endif()

elseif (ANDROID)
    message(STATUS "Detected platform: Android")
    # Android-specific settings
    set(PLATFORM_NAME "Android")
    # Add more Android-specific configurations or libraries here

elseif (UNIX)
    message(STATUS "Detected platform: Linux")
    # Linux-specific settings
    set(PLATFORM_NAME "Linux")
    # Add more Linux-specific configurations or libraries here

else()
    message(FATAL_ERROR "Unknown platform: Build not supported on this platform.")
endif()

# Use the PLATFORM_NAME as needed in your project
message(STATUS "Configured for platform: ${PLATFORM_NAME}")

if (PLATFORM_NAME STREQUAL "Windows")
    target_compile_definitions(BloxxEngine PRIVATE BE_PLATFORM_WINDOWS)
    # Windows-specific compile options or link libraries

elseif (PLATFORM_NAME STREQUAL "macOS")
    target_compile_definitions(BloxxEngine PRIVATE BE_PLATFORM_MACOS)
    # macOS-specific compile options or link libraries

elseif (PLATFORM_NAME STREQUAL "iOS")
    target_compile_definitions(BloxxEngine PRIVATE BE_PLATFORM_IOS)
    # iOS-specific compile options or link libraries

elseif (PLATFORM_NAME STREQUAL "Android")
    target_compile_definitions(BloxxEngine PRIVATE BE_PLATFORM_ANDROID)
    # Android-specific compile options or link libraries

elseif (PLATFORM_NAME STREQUAL "Linux")
    target_compile_definitions(my_target PRIVATE BE_PLATFORM_LINUX)
    # Linux-specific compile options or link libraries
    target_link_libraries(BloxxEngine stdc++ GL)
endif()
target_link_libraries(BloxxEngine glfw ImGui spdlog glad glm stb Taskflow FastNoise2)

target_include_directories(BloxxEngine PUBLIC include)


set_target_properties(glfw PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(ImGui PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(spdlog PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(glad PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(glm PROPERTIES EXCLUDE_FROM_ALL 1)
set_target_properties(stb PROPERTIES EXCLUDE_FROM_ALL 1)