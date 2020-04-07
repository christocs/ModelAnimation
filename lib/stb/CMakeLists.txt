project(stb)

add_library(stb)

target_sources(stb PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/stb_image.cpp)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/stb_image.cpp
"#define STB_IMAGE_IMPLEMENTATION\n\
#include \"stb_image.h\"")

target_include_directories(stb PUBLIC . PRIVATE stb)
