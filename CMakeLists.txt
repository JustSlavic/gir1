cmake_minimum_required(VERSION 3.14)
project(gir1 VERSION 0.0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(libs)

add_subdirectory(libs/stb)
add_subdirectory(libs/imgui)
add_subdirectory(tests)

set(HEADER_FILES
        include/defines.h
        include/index_buffer.h
        include/renderer.h
        include/shader.h
        include/texture.h
        include/input.h
        include/camera.h
        include/model.h
        include/utils.h
        include/version.h
        include/vertex_array.h
        include/vertex_buffer.h
        include/vertex_buffer_layout.h)

set(SOURCE_FILES
        src/index_buffer.cpp
        src/renderer.cpp
        src/shader.cpp
        src/texture.cpp
        src/input.cpp
        src/camera.cpp
        src/model.cpp
        src/utils.cpp
        src/version.cpp
        src/vertex_array.cpp
        src/vertex_buffer.cpp
        src/vertex_buffer_layout.cpp)


find_library(GL REQUIRED)
find_library(GLU REQUIRED)
find_library(glfw REQUIRED)
find_library(GLEW REQUIRED)


add_executable(gir1 main.cpp
    ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(gir1 GL GLU glfw GLEW stb imgui)

add_executable(sandbox sandbox.cpp)

target_link_libraries(sandbox GL GLU glfw GLEW)
