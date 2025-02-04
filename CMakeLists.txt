cmake_minimum_required(VERSION 3.10)
project(TFCraft VERSION 0.0.1)

find_package(OpenGL REQUIRED)

set(FASTNOISE2_BUILD_SHARED ON)
add_subdirectory(TFCraft-libs/FastNoise2)
add_subdirectory(TFCraft-libs/glfw-3.4)


include_directories(
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/glew-2.1.0/include
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/FastNoise2/include
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/glfw-3.4/include
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/glm-1.0.1
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/stb
)



if(LINUX)
    link_libraries(
        ${PROJECT_SOURCE_DIR}/build/TFCraft-libs/FastNoise2/src/libFastNoise.a
        ${PROJECT_SOURCE_DIR}/build/TFCraft-libs/glfw-3.4/src/libglfw3.a
        )
endif()


add_executable(
    TFCraft
    ${PROJECT_SOURCE_DIR}/src/engine/texture.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/engine.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/shader.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/timer.cpp
    ${PROJECT_SOURCE_DIR}/src/colliders.cpp
    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(TFCraft
    FastNoise
    OpenGL::GL
    glfw
    GLEW
)

target_compile_definitions(TFCraft PRIVATE GLEW_STATIC)