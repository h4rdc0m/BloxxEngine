add_executable(Sandbox src/Application.cpp)
target_link_libraries(Sandbox BloxxEngine)


add_custom_command(TARGET Sandbox PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:Sandbox>/Resources)