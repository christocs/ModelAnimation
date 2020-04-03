project(lua)

add_library(lua STATIC
    lua/lauxlib.c
    lua/liolib.c
    lua/lopcodes.c
    lua/lstate.c
    lua/lobject.c
    lua/lmathlib.c
    lua/loadlib.c
    lua/lvm.c
    lua/lfunc.c
    lua/lstrlib.c
    lua/lua.c
    lua/linit.c
    lua/lstring.c
    lua/lundump.c
    lua/lctype.c
    lua/ltable.c
    lua/ldump.c
    lua/loslib.c
    lua/lgc.c
    lua/lzio.c
    lua/ldblib.c
    lua/lutf8lib.c
    lua/lmem.c
    lua/lcorolib.c
    lua/lcode.c
    lua/ltablib.c
    lua/lbitlib.c
    lua/lapi.c
    lua/lbaselib.c
    lua/ldebug.c
    lua/lparser.c
    lua/llex.c
    lua/ltm.c
    lua/ldo.c
)

target_include_directories(lua PUBLIC .)
