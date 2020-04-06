add_library(glad
    src/glad.cpp
)

target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT_BUILD=1)

target_include_directories(glad PUBLIC include)
