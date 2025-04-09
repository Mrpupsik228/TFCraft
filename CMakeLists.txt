cmake_minimum_required(VERSION 3.10)
project(TFCraft VERSION 0.0.1)

include(FetchContent)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

message("Fetching GLFW")
FetchContent_Declare(
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG "origin/master"
)
FetchContent_MakeAvailable(glfw)

message("Fetching glm")
FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG "origin/master"
)
FetchContent_MakeAvailable(glm)

message("Fetching stb")
FetchContent_Declare(
  stb
  GIT_REPOSITORY "https://github.com/nothings/stb.git"
  GIT_TAG "origin/master"
)
FetchContent_MakeAvailable(stb)

# Add your executable sources
add_executable(TFCraft
    ${PROJECT_SOURCE_DIR}/src/glad.c
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/FastNoise.cpp
    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/engine.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/shader.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/texture.cpp
    ${PROJECT_SOURCE_DIR}/src/engine/timer.cpp
)

# Use target_include_directories to add any additional include paths that are not provided by the fetched packages.
target_include_directories(TFCraft PRIVATE
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/glfw3.4/include
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/glad/include
    ${PROJECT_SOURCE_DIR}/TFCraft-libs/stb    # if you need to manually add stb includes
)

# Link using the targets provided by the packages
target_link_libraries(TFCraft PRIVATE
    OpenGL::GL
    glfw
    glm
)