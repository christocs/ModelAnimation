# Add source files.
target_sources(${PROJECT_NAME} PRIVATE
    Main.cpp
    Model.cpp
    ModelLoader.cpp
    Shader.cpp
    Mesh.cpp
    Camera.cpp
)

# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
