project(fast-noise-simd)

add_library(fast-noise-simd)

target_sources(fast-noise-simd PRIVATE
    fast-noise-simd/FastNoiseSIMD/FastNoiseSIMD_sse2.cpp
    fast-noise-simd/FastNoiseSIMD/FastNoiseSIMD_sse41.cpp
    fast-noise-simd/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp
    fast-noise-simd/FastNoiseSIMD/FastNoiseSIMD_internal.cpp
    fast-noise-simd/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp
    fast-noise-simd/FastNoiseSIMD/FastNoiseSIMD_neon.cpp
    fast-noise-simd/FastNoiseSIMD/FastNoiseSIMD.cpp
)

target_compile_options(fast-noise-simd PRIVATE -march=core-avx2)

target_include_directories(fast-noise-simd PUBLIC fast-noise-simd/FastNoiseSIMD)
