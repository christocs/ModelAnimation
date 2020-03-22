#pragma once
#include <string>
#include <vector>

#include <SFML/Window/Mouse.hpp>
namespace LuaMouse {
  struct Mouse {
    std::string name;
    int button;
  };
  auto getButtons() -> std::vector<Mouse> &;
}
