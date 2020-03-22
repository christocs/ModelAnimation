#pragma once
#include <string>
#include <vector>

#include <SFML/Window/Mouse.hpp>
/**
 * Basically acts as a façade between sf::Mouse and Lua
 */
namespace LuaMouse {
  struct Mouse {
    std::string name;
    int button;
  };
  /**
   * Return not const as references to these values are used by lua.
   * Despite being "readonly" from lua's side, they can't be const.
   * (Even readonly variables can be changed in lua, with things like rawset.)
   */
  auto getButtons() -> std::vector<Mouse> &;
}
