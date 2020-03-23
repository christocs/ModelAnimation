function(target_ignore_header_warnings target scope)
    get_target_property(lib_include_dirs ${target} INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${target} SYSTEM ${scope} ${lib_include_dirs})
endfunction()

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
add_subdirectory(freetype)

# DLLs were a mistake.
set_target_properties(reactphysics3d PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(reactphysics3d PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Ignore header warnings in our libraries.
target_ignore_header_warnings(EnTT INTERFACE)
target_ignore_header_warnings(assimp INTERFACE)
target_ignore_header_warnings(reactphysics3d INTERFACE)
target_ignore_header_warnings(glad INTERFACE)
target_ignore_header_warnings(cpplocate INTERFACE)
target_ignore_header_warnings(lua INTERFACE)
target_ignore_header_warnings(LuaBridge INTERFACE)
target_ignore_header_warnings(glm INTERFACE)
target_ignore_header_warnings(sfml-system INTERFACE)
