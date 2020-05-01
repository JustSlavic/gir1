cmake_minimum_required(VERSION 3.14)
project(gir1 VERSION 0.0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(HEADER_FILES
        include/defines.h
        include/index_buffer.h
        include/renderer.h
        include/utils.h
        include/version.h
        include/vertex_array.h
        include/vertex_buffer.h
        include/vertex_buffer_layout.h)

set(SOURCE_FILES
        src/index_buffer.cpp
        src/renderer.cpp
        src/utils.cpp
        src/version.cpp
        src/vertex_array.cpp
        src/vertex_buffer.cpp src/vertex_buffer_layout.cpp)


find_library(GL REQUIRED)
find_library(GLU REQUIRED)
find_library(glfw REQUIRED)
find_library(GLEW REQUIRED)


add_executable(gir1 main.cpp
    ${HEADER_FILES} ${SOURCE_FILES})


target_link_libraries(gir1 GL GLU glfw GLEW)
