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
    LuaRef onUpdate;
    // Keyboard
    LuaRef onKeyPress;
    LuaRef onKeyRelease;
    LuaRef onTextEnter;
    // Mouse
    LuaRef onMouseMove;
    LuaRef onMouseScroll;
    LuaRef onMousePress;
    LuaRef onMouseRelease;

    std::filesystem::file_time_type lastFileUpdate;

  public:
    static auto setupLuaState(lua_State *lua) -> void;

    ScriptComponent(lua_State *lua, const std::string &filename);

    // Reload file
    auto reload(lua_State *lua) -> void;
    auto reloadIfOld(lua_State *lua) -> void;

    // Assuming DT is float for now, will change if needed.
    auto update(float dt) -> void;
    // auto keyPress(sf::Keyboard::Key key, bool alt, bool ctrl, bool shift) -> void;
    // auto keyRelease(sf::Keyboard::Key key, bool alt, bool ctrl, bool shift) -> void;
    auto textEnter(const std::string &text) -> void;
    auto mouseMove(int mousex, int mousey) -> void;
    // No point in supporting multiple mouse wheels (although SFML does)
    auto mouseScroll(float delta, int mousex, int mousey) -> void;
    // auto mousePress(sf::Mouse::Button button, int mousex, int mousey) -> void;
    // auto mouseRelease(sf::Mouse::Button button, int mousex, int mousey) -> void;
  };
}
