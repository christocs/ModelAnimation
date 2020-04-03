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
    std::filesystem::path scriptPath;
    // Engine
    LuaRef on_update;
    // Keyboard
    LuaRef on_key_press;
    LuaRef on_key_release;
    LuaRef on_tex_tEnter;
    // Mouse
    LuaRef on_mouse_move;
    LuaRef on_mouse_scroll;
    LuaRef on_mouse_press;
    LuaRef on_mouse_release;

    std::filesystem::file_time_type last_file_update;

  public:
    static auto setup_lua_state(lua_State *lua) -> void;

    ScriptComponent(lua_State *lua, const std::string &filename);

    // Reload file
    auto reload(lua_State *lua) -> void;
    auto reload_if_old(lua_State *lua) -> void;

    // Assuming DT is float for now, will change if needed.
    auto update(float dt) -> void;
    // auto key_press(sf::Keyboard::Key key, bool alt, bool ctrl, bool shift) -> void;
    // auto key_release(sf::Keyboard::Key key, bool alt, bool ctrl, bool shift) -> void;
    auto text_enter(const std::string &text) -> void;
    auto mouse_move(int mousex, int mousey) -> void;
    // No point in supporting multiple mouse wheels (although SFML does)
    auto mouse_scroll(float delta, int mousex, int mousey) -> void;
    // auto mouse_press(sf::Mouse::Button button, int mousex, int mousey) -> void;
    // auto mouse_release(sf::Mouse::Button button, int mousex, int mousey) -> void;
  };
}
