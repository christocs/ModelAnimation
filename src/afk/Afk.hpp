#pragma once

#include <glm/glm.hpp>

#include "afk/renderer/Camera.hpp"
#include "afk/renderer/Renderer.hpp"

namespace Afk {
  class Engine {
  public:
    static constexpr const char *GAME_NAME = "ICT397";

    Renderer renderer = {};
    Camera camera     = {};

    Engine();
    Engine(Engine &&)      = delete;
    Engine(const Engine &) = delete;
    auto operator=(const Engine &) -> Engine & = delete;
    auto operator=(Engine &&) -> Engine & = delete;

    static auto get() -> Engine &;

    auto render() -> void;
    auto update() -> void;

    auto get_time() -> float;
    auto get_delta_time() -> float;
    auto get_is_running() const -> bool;

    auto handle_keys() -> void;
    auto handle_mouse() -> void;

  private:
    bool is_running       = true;
    unsigned fps_count    = {};
    float last_update     = {};
    float last_fps_update = {};
  };
}
