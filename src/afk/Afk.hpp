#pragma once

#include <SFML/System/Clock.hpp>
#include <glm/glm.hpp>

#include "afk/render/Camera.hpp"
#include "afk/render/Renderer.hpp"

namespace Afk {
  class Engine {
  public:
    static constexpr const char *GAME_NAME = "ICT397";

    Renderer renderer = {};
    Camera camera     = {};

    auto render() -> void;
    auto update() -> void;

    auto getTime() -> float;
    auto getDeltaTime() -> float;
    auto getIsRunning() const -> bool;

    auto handleKeys() -> void;
    auto handleMouse() -> void;

  private:
    sf::Clock clock     = {};
    bool isRunning      = true;
    bool hasFocus       = true;
    unsigned fpsCount   = {};
    float lastUpdate    = {};
    float lastFpsUpdate = {};
  };
}
