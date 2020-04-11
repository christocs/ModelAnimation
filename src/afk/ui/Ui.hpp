#pragma once

#include <memory>
#include <unordered_map>

#include <imgui/imgui.h>

#include "afk/renderer/Renderer.hpp"
#include "afk/ui/Log.hpp"

// FIXME: Add log, texture/mesh viewer
namespace Afk {
  class Ui {
  public:
    static constexpr auto FONT_SIZE = 19.0f;

    bool show_menu = false;
    Log log        = {};

    Ui(Renderer::Window _window);
    ~Ui();

    auto open() -> void;
    auto close() -> void;
    auto draw() -> void;
    auto prepare() const -> void;

  private:
    std::string ini_path    = {};
    Renderer::Window window = {};

    bool show_stats = true;
    bool show_demo  = false;
    bool show_about = false;
    bool show_log   = false;
    float scale     = 1.5f;

    std::unordered_map<std::string, ImFont *> fonts = {};

    auto draw_menu_bar() -> void;
    auto draw_stats() -> void;
    auto draw_about() -> void;
    auto draw_log() -> void;
  };
}
