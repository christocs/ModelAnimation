#pragma once

#include <reactphysics3d.h>

#include <entt/entt.hpp>

#include "afk/physics/PhysicsBody.hpp"
#include "glm/vec3.hpp"

namespace Afk {
  class PhysicsBody;

  using World = rp3d::DynamicsWorld;

  class PhysicsBodySystem {
    public:
      PhysicsBodySystem();

      PhysicsBodySystem(glm::vec3 gravity);

      auto GetGravity();

      auto SetGravity(glm::vec3 gravity);

      auto update(entt::registry* registry, float dt) -> void;

    private:
      World* world;

    friend class PhysicsBody;
  };
};
