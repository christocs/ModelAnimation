#pragma once

#include <memory>

#include <imgui/imgui.h>

#include "afk/renderer/Renderer.hpp"

namespace Afk {
  class Ui {
  public:
    Ui(Renderer::Window _window);
    ~Ui();

    auto draw() -> void;
    auto prepare() -> void;

    std::string ini_path    = {};
    bool is_visible         = false;
    Renderer::Window window = {};
  };
}
