# Assimp build options.
SET(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build assimp tests.")
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build assimp tools.")
SET(INJECT_DEBUG_POSTFIX OFF CACHE BOOL "Append 'd' to debug library.")

# Build libraries.
add_compile_options("-Wno-everything")

add_compile_options(
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-everything>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-w>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/w>
)


add_subdirectory(entt EXCLUDE_FROM_ALL)
add_subdirectory(sfml EXCLUDE_FROM_ALL)
add_subdirectory(assimp EXCLUDE_FROM_ALL)
add_subdirectory(react EXCLUDE_FROM_ALL)
