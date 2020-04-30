cmake_minimum_required(VERSION 3.14)

project(fastnoise-simd)

add_library(fastnoise-simd)
target_sources(fastnoise-simd PRIVATE
    fastnoise-simd/FastNoiseSIMD/FastNoiseSIMD.cpp
    fastnoise-simd/FastNoiseSIMD/FastNoiseSIMD_internal.cpp
)

option(FASTNOISE_SSE2 "Enable SSE2" ON)
option(FASTNOISE_SSE41 "Enbale SSE4.1" OFF)
option(FASTNOISE_AVX2 "Enable AVX2" OFF)
option(FASTNOISE_AVX512 "Enable AVX512" OFF)
option(FASTNOISE_NEON "Enable Neon" OFF)
option(FASTNOISE_USE_FMA "Enable FMA" OFF)
option(FASTNOISE_ALIGNED_SETS "Enable aligned sets" OFF)
option(FASTNOISE_NO_SIMD_FALLBACK "Disable SIMD fallback" OFF)

if(FASTNOISE_SSE2)
    target_compile_options(fastnoise-simd PRIVATE
       $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2>
       $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-msse2>
    )

    target_sources(fastnoise-simd PRIVATE fastnoise-simd/FastNoiseSIMD/FastNoiseSIMD_sse2.cpp)
    target_compile_definitions(fastnoise-simd PRIVATE FN_COMPILE_SSE2=1)
endif()

if(FASTNOISE_SSE41)
    target_compile_options(fastnoise-simd PRIVATE
       $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2>
       $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-msse4.1>
    )

    target_sources(fastnoise-simd PRIVATE fastnoise-simd/FastNoiseSIMD/FastNoiseSIMD_sse41.cpp)
    target_compile_definitions(fastnoise-simd PRIVATE FN_COMPILE_SSE41=1)
endif()

if(FASTNOISE_AVX2)
    target_compile_options(fastnoise-simd PRIVATE
       $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
       $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-mavx2>
    )

    target_sources(fastnoise-simd PRIVATE fastnoise-simd/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp)
    target_compile_definitions(fastnoise-simd PRIVATE FN_COMPILE_AVX2=1)
endif()

if(FASTNOISE_AVX512)
    target_compile_options(fastnoise-simd PRIVATE
       $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
       $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-mavx512f>
    )

    target_sources(fastnoise-simd PRIVATE fastnoise-simd/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp)
    target_compile_definitions(fastnoise-simd PRIVATE FN_COMPILE_AVX512=1)
endif()

if(FASTNOISE_NEON)
    target_sources(fastnoise-simd PRIVATE fastnoise-simd/FastNoiseSIMD/FastNoiseSIMD_neon.cpp)
    target_compile_definitions(fastnoise-simd PRIVATE FN_COMPILE_NEON=1)
endif()

if(FASTNOISE_USE_FMA)
    target_compile_definitions(fastnoise-simd PRIVATE FN_USE_FMA=1)
endif()

if(FASTNOISE_ALIGNED_SETS)
    target_compile_definitions(fastnoise-simd PRIVATE FN_ALIGNED_SETS=1)
endif()

if(FASTNOISE_NO_SIMD_FALLBACK)
    target_compile_definitions(fastnoise-simd PRIVATE FN_COMPILE_NO_SIMD_FALLBACK=1)
endif()

target_include_directories(fastnoise-simd INTERFACE fastnoise-simd)
set_target_properties(fastnoise-simd PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(fastnoise-simd PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
