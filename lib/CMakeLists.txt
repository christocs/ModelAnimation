# Assimp build options.
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(INJECT_DEBUG_POSTFIX OFF)

# Build libraries.
string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_compile_options(
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-everything>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-w>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
)

add_subdirectory(entt EXCLUDE_FROM_ALL)
add_subdirectory(sfml EXCLUDE_FROM_ALL)
add_subdirectory(assimp EXCLUDE_FROM_ALL)
add_subdirectory(glad EXCLUDE_FROM_ALL)
add_subdirectory(cpplocate EXCLUDE_FROM_ALL)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_subdirectory(react EXCLUDE_FROM_ALL)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

# Ignore warnings from library headers.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    entt/src
    sfml/include
    assimp/include
    react/src
    glad/include
)

