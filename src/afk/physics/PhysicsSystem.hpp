#pragma once

#include <entt/entt.hpp>
#include <reactphysics3d.h>

namespace Afk {
  auto update_physics(entt::registry* registry, rp3d::DynamicsWorld* world, float dt) -> void;
};
