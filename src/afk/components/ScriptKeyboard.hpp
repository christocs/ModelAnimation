#pragma once
#include <string>
#include <vector>

#include <SFML/Window/Keyboard.hpp>
namespace LuaKeyboard {
  struct Key {
    std::string name;
    int code;
  };
  auto getKeys() -> std::vector<Key> &;
}
