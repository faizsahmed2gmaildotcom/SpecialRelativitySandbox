cmake_minimum_required(VERSION 4.1.2)
project(OpenGL-Tutorial)

set(CMAKE_CXX_STANDARD 26)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(OpenGL-Tutorial
        ../glad/src/glad.c
        main.cpp
)

target_include_directories(OpenGL-Tutorial PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(OpenGL-Tutorial PRIVATE ${GLFW_LIBRARIES})
include_directories(../glad/include)
