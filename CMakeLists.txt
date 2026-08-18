cmake_minimum_required(VERSION 4.1.2)
project(SpecialRelativitySandbox)
add_compile_options(-O3)  # Max. runtime speed

set(CMAKE_CXX_STANDARD 26)
add_library(VecMat)
target_sources(VecMat PUBLIC FILE_SET CXX_MODULES FILES vector_matrix.ixx)

add_executable(SpecialRelativitySandbox main.cpp)
target_link_libraries(SpecialRelativitySandbox PRIVATE VecMat)
