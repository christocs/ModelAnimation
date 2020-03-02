# Set cmake minimum version.
cmake_minimum_required(VERSION 3.14)

# Set the project details.
set(PROJECT_NAME ict397)
project(${PROJECT_NAME} LANGUAGES CXX)

# Clang sanitizer settings.
set(SANITIZER_OS "Darwin,Linux")
set(SANITIZER_FLAGS "-fsanitize=address,undefined,leak")

# Treat warnings as errors.
option(WarningsAsErrors "WarningsAsErrors" OFF)

# Disable in-source builds.
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set build output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/out/lib)

SET(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build assimp tests.")
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Build assimp tools.")
SET(INJECT_DEBUG_POSTFIX OFF CACHE BOOL "Append 'd' to debug library.")

# Build libraries.
add_subdirectory(lib/entt)
add_subdirectory(lib/SFML)
add_subdirectory(lib/assimp)
add_subdirectory(lib/react)

# Define the executable.
add_executable(${PROJECT_NAME}
    src/Main.cpp
)

# Ignore warnings from libraries.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    lib/entt/src
    lib/sfml/include
    lib/assimp/include
    lib/react/src
)

# Set C++ standard.
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Remove the default warning level from MSVC.
if (MSVC)
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

# Treat warnings as errors if enabled.
if (WarningsAsErrors)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:
            /WX>
    )
endif()

# Set compile flags.
target_compile_options(${PROJECT_NAME} PRIVATE
    # Clang
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        -Weverything -fcolor-diagnostics
        # Disable specific warnings.
        -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
        -Wno-deprecated-declarations -Wno-exit-time-destructors
        -Wno-switch-enum -Wno-weak-vtables>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:
        -Wall -Wextra -Wpedantic -fdiagnostics-color=always>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4>
    # Enable the clang address and memory sanitizer.
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:${SANITIZER_OS}>>:
        ${SANITIZER_FLAGS}>
)

# Enable the clang address and memory sanitizer.
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:${SANITIZER_OS}>>:
        ${SANITIZER_FLAGS}>
)

# Find dependencies.
find_package(OpenGL COMPONENTS OpenGL REQUIRED)

# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Include and link against dependencies.
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL OpenGL::GLU EnTT::EnTT
    sfml-network sfml-network sfml-graphics sfml-window sfml-system
    assimp reactphysics3d)

# Symlink resources to the binary location.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
