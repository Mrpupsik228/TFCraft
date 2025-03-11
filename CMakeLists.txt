cmake_minimum_required(VERSION 3.10)
project(TFCraft VERSION 0.0.1)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(FASTNOISE2_BUILD_SHARED ON)
add_subdirectory(libs/FastNoise2)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)

include_directories(
    ${PROJECT_SOURCE_DIR}/libs/FastNoise2/include
    ${PROJECT_SOURCE_DIR}/libs/glad/include
    ${PROJECT_SOURCE_DIR}/libs/glfw/include
    ${PROJECT_SOURCE_DIR}/libs/glm
    ${PROJECT_SOURCE_DIR}/libs/stb
)

link_libraries(
    ${PROJECT_SOURCE_DIR}/build/libs/FastNoise2/src/libFastNoise.a
    ${PROJECT_SOURCE_DIR}/build/libs/glfw/src/libglfw3.a
    ${PROJECT_SOURCE_DIR}/build/libs/glm/glm/libglm.a
)

add_executable(TFCraft
    ${PROJECT_SOURCE_DIR}/src/engine/graphics/framebuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/graphics/texture.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/graphics/shader.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/graphics/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/util/physics.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/util/maths.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/util/time.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/io/window.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/io/logger.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/engine.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/glad.c
)

target_link_libraries(TFCraft
    OpenGL::GL
    FastNoise
    glfw
    glm
)