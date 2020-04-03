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

    auto getTime() -> float;
    auto getDeltaTime() -> float;
    auto getIsRunning() const -> bool;

    auto handleKeys() -> void;
    auto handleMouse() -> void;

  private:
    bool isRunning      = true;
    unsigned fpsCount   = {};
    float lastUpdate    = {};
    float lastFpsUpdate = {};
  };
}
