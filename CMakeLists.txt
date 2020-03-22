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

# Disable debug library postfix.
set(CMAKE_DEBUG_POSTFIX "")

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add Linux flag.
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Set build output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG    ${PROJECT_BINARY_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${PROJECT_BINARY_DIR}/out/lib)

# Define the executable.
add_executable(${PROJECT_NAME})

# Set C++ standard.
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Add source files.
add_subdirectory(src)
# Add third party libraries.
add_subdirectory(lib)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    lib/lua-bridge/Source/LuaBridge
    lib/lua-bridge/Source/LuaBridge/detail
)


# Remove the default warning level from MSVC.
if (MSVC)
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# Treat warnings as errors if enabled.
if (WarningsAsErrors)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
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
        -Wno-switch-enum -Wno-weak-vtables -Wno-global-constructors>
    # GCC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -fdiagnostics-color=always>
    # Visual Studio
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    # Enable the clang sanitizer.
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:${SANITIZER_OS}>>:${SANITIZER_FLAGS}>
)

# Link against the clang sanitizer.
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:${SANITIZER_OS}>>:${SANITIZER_FLAGS}>
)

# Find dependencies.
find_package(OpenGL COMPONENTS OpenGL REQUIRED)

# Include and link against dependencies.
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    EnTT::EnTT
    assimp
    reactphysics3d
    glad
    cpplocate
    lua
    LuaBridge
    glm
    sfml-audio sfml-graphics sfml-network sfml-system sfml-window
    $<$<OR:$<PLATFORM_ID:Windows>,$<PLATFORM_ID:Linux>>:freetype>
    $<$<PLATFORM_ID:Windows>:sfml-main>
)

# Symlink resources to the binary location.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/src/afk/render/shader $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader)
