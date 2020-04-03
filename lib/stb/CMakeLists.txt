project(stb)

add_library(stb STATIC
    stb/stb_vorbis.c
)

target_include_directories(stb PUBLIC .)
