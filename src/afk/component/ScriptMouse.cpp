#include "ScriptMouse.hpp"

#include <GLFW/glfw3.h>

auto LuaMouse::get_buttons() -> std::vector<Mouse> & {
  static std::vector<Mouse> s{
      {"Left", GLFW_MOUSE_BUTTON_1},   {"Right", GLFW_MOUSE_BUTTON_2},
      {"Middle", GLFW_MOUSE_BUTTON_3}, {"Mouse4", GLFW_MOUSE_BUTTON_4},
      {"Mouse5", GLFW_MOUSE_BUTTON_5}, {"Mouse6", GLFW_MOUSE_BUTTON_6},
      {"Mouse7", GLFW_MOUSE_BUTTON_7}, {"Mouse8", GLFW_MOUSE_BUTTON_8},
  };
  return s;
}
