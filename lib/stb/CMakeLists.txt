cmake_minimum_required(VERSION 3.16)

project(stb)
add_library(stb)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/stb_image.cpp
"#define STB_IMAGE_IMPLEMENTATION\n\
#include \"stb_image.h\"")

set_target_properties(stb PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(stb PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_sources(stb PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/stb_image.cpp)
target_include_directories(stb PUBLIC . PRIVATE stb)
