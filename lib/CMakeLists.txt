# Library build options.
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(OPTION_BUILD_TESTS OFF)

# Build libraries.
string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Disable compiler warnings.
add_compile_options(
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-everything>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-w>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
)

add_subdirectory(entt)
add_subdirectory(sfml)
add_subdirectory(assimp)
add_subdirectory(glad)
add_subdirectory(cpplocate)
add_subdirectory(reactphysics3d)
add_subdirectory(lua)
add_subdirectory(lua-bridge)
add_subdirectory(glm)

if (WIN32 OR LINUX)
    add_subdirectory(freetype)
endif()

# DLL's were a mistake.
set_target_properties(reactphysics3d PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(reactphysics3d PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Ignore warnings from library headers.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    entt/src
    sfml/include
    assimp/include
    react/src
    glad/include
    lua/src
    lua-bridge/Source/LuaBridge
    freetype/include
)
