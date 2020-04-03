#pragma once
#include <string>
#include <vector>

/**
 * Basically acts as a façade between sf::Keyboard and Lua
 */
namespace LuaKeyboard {
  struct Key {
    std::string name;
    int code;
  };
  /**
   * Return not const as references to these values are used by lua.
   * Despite being "readonly" from lua's side, they can't be const.
   * (Even readonly variables can be changed in lua, with things like rawset.)
   */
  auto getKeys() -> std::vector<Key> &;
}
