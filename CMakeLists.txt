cmake_minimum_required(VERSION 4.1.2)
project(SpecialRelativitySandbox)
add_compile_options(-O3)  # Min. runtime speed

set(CMAKE_CXX_STANDARD 26)

add_executable(SpecialRelativitySandbox main.cpp objects.h relativity.h vector_matrix.h)
