#pragma once

#include <memory>

#include <imgui/imgui.h>

#include "afk/renderer/Renderer.hpp"

// FIXME: Add log, texture/mesh viewer
namespace Afk {
  class Ui {
  public:
    bool is_visible = false;

    Ui(Renderer::Window _window);
    ~Ui();

    auto open() -> void;
    auto close() -> void;
    auto draw() -> void;
    auto prepare() const -> void;

  private:
    std::string ini_path    = {};
    Renderer::Window window = {};
    bool show_stats         = true;
    bool show_demo          = false;
    bool show_about         = false;

    auto draw_menu_bar() -> void;
    auto draw_stats() -> void;
    auto draw_about() -> void;
  };
}
