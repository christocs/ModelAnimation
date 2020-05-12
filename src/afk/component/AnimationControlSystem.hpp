#pragma once

#include <entt/entt.hpp>

namespace Afk {
  class AnimationControlSystem {
  public:
    auto update(entt::registry *registry, double dt) -> void;

    constexpr const auto static move_speed = 1.0f;

  private:
    double current_animation_time = 0.0;
  };
}
