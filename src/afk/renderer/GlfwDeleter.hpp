#pragma once

#include <GLFW/glfw3.h>

namespace std {
  template<>
  class default_delete<GLFWwindow> {
  public:
    void operator()(GLFWwindow *ptr) {
      glfwDestroyWindow(ptr);
    }
  };
}
