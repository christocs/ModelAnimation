#pragma once

#include <entt/entt.hpp>
#include "afk/renderer/Renderer.hpp"

namespace Afk {
  auto queue_models(entt::registry* registry, Afk::Renderer* renderer) -> void;
};
