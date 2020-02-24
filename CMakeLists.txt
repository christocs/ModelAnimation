# Set cmake minimum version.
cmake_minimum_required(VERSION 3.14)

# Set the project details.
set(PROJECT_NAME ict397)
project(${PROJECT_NAME} LANGUAGES CXX)

# Clang sanitizer flags.
set(SANITIZER_FLAGS "-fsanitize=address,undefined,leak")
set(SANITIZER_OS "Darwin,Linux")

# If enabled, the post build symlink will copy instead.
option(CopyResources "CopyResources" OFF)
# Disables any post build steps.
option(DisablePostBuild "DisablePostBuild" OFF)
# Treat warnings as errors.
option(WarningsAsErrors "WarningsAsErrors" OFF)

# Disable in-source builds.
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

# Set the cmake module paths.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Define source files.
set (SOURCES
    src/Main.cpp
)

# Define the executable.
add_executable(${PROJECT_NAME} ${SOURCES})

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
    )

    target_compile_options(${PROJECT_NAME} PRIVATE
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
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Include and link against dependencies.
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL OpenGL::GLU)

# Symlink or copy the resources to the binary location.
if (NOT DisablePostBuild)
    if (NOT CopyResources)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
    else()
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
    endif()
endif()
