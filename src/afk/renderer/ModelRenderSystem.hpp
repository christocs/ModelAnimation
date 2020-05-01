#pragma once

#include <entt/entt.hpp>
#include "afk/renderer/Renderer.hpp"

namespace Afk {
  auto queue_models(entt::registry* registry, Afk::Renderer* renderer, const std::filesystem::path shader_program_path) -> void;
};
