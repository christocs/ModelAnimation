#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

extern "C" {
#include <lua/lauxlib.h>
#include <lua/lua.h>
#include <lua/lualib.h>
}

#include <LuaBridge/LuaBridge.h>

using luabridge::LuaRef;
struct lua_State;

namespace Afk {
  class ScriptComponent {
  private:
    std::filesystem::path file_path;
    // Engine
    LuaRef on_update;
    // Keyboard
    LuaRef on_key_press;
    LuaRef on_key_release;
    LuaRef on_text_enter;
    // Mouse
    LuaRef on_mouse_move;
    LuaRef on_mouse_scroll;
    LuaRef on_mouse_press;
    LuaRef on_mouse_release;

    std::filesystem::file_time_type last_file_update;

  public:
    static auto setup_lua_state(lua_State *lua) -> void;

    ScriptComponent(lua_State *lua, const std::filesystem::path &file_name);

    // Reload file
    auto reload(lua_State *lua) -> void;
    auto reload_if_old(lua_State *lua) -> void;

    auto update(float dt) -> void;
    auto key_press(int key, bool alt, bool ctrl, bool shift) -> void;
    auto key_release(int key, bool alt, bool ctrl, bool shift) -> void;
    auto text_enter(const std::string &text) -> void;
    auto mouse_move(int x, int y) -> void;
    auto mouse_scroll(float delta) -> void;
    auto mouse_press(int button) -> void;
    auto mouse_release(int button) -> void;
  };
}
