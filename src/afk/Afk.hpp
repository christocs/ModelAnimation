#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "afk/render/Camera.hpp"
#include "afk/render/Model.hpp"
#include "afk/render/Shader.hpp"

namespace Afk {
    class Engine {
      public:
        using Window = sf::Window;
        using Clock  = sf::Clock;
        using Time   = sf::Time;
        using Models = std::vector<Render::Model>;

        // FIXME: Decouple and split into subsystems.
        Render::Camera camera = {};
        Window window         = {};
        Render::Shader shader = {};
        Models models         = {};

        auto handleEvents() -> void;
        auto update() -> void;
        auto render() -> void;

        auto getIsRunning() const -> bool;

        Engine();

      private:
        Clock clock      = {};
        float lastUpdate = {};
        bool isRunning   = true;

        auto getDeltaTime() const -> float;
        auto getTime() const -> float;
        auto handleKeys() -> void;
        auto handleMouse() -> void;
    };
};
