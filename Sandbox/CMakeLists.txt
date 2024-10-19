add_executable(Sandbox src/main.cpp)
target_link_libraries(Sandbox BloxxEngine SDL3::SDL3)
if(WIN32)
    add_custom_command(
            TARGET Sandbox POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:Sandbox>
            VERBATIM
    )
endif()