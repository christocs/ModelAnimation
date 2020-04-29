#pragma once

#include <entt/entt.hpp>
#include "afk/renderer/Renderer.hpp"

namespace Afk {
  auto render_models(entt::registry* registry, Afk::Renderer* renderer, const Afk::ShaderProgramHandle* shader) -> void;
};
