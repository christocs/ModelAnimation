# Add source files.
target_sources(${PROJECT_NAME} PRIVATE
    Main.cpp
    afk/Afk.cpp
    afk/physics/Transform.cpp
    afk/io/Path.cpp
    afk/io/Log.cpp
    afk/io/ModelLoader.cpp
    afk/render/Camera.cpp
    afk/render/ShaderData.cpp
    afk/render/TextureData.cpp
    afk/render/ModelData.cpp
    afk/render/opengl/Renderer.cpp
    afk/components/ScriptComponent.cpp
    afk/components/ScriptKeyboard.cpp
    afk/components/ScriptMouse.cpp
)


# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
