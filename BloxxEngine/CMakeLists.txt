set(TARGET_NAME BloxxEngine)

# Collect sources
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_23)

target_include_directories(${TARGET_NAME} PUBLIC src)
