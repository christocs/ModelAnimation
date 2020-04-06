project(stb)

add_library(stb)

target_sources(stb PRIVATE
    Dummy.cpp
    stb/stb_vorbis.c
)

target_include_directories(stb PUBLIC .)
