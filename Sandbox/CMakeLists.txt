set(TARGET_NAME Sandbox)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_23)

target_link_libraries(${TARGET_NAME} PRIVATE BloxxEngine)
