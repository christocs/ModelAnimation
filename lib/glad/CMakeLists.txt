add_library(glad STATIC
    src/glad.cpp
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
