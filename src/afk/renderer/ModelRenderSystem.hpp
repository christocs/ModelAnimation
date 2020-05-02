#pragma once

#include <entt/entt.hpp>

#include "afk/renderer/Renderer.hpp"

namespace Afk {
  /**
   * render all models for things with model components
   */
  auto queue_models(entt::registry *registry, Afk::Renderer *renderer) -> void;
};
