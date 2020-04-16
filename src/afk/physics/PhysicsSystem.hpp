#pragma once

#include "afk/physics/Collision.hpp"

#include "glm/vec3.hpp"
#include <entt/entt.hpp>
#include <reactphysics3d.h>

namespace Afk {
  class Collision;

  using World = rp3d::DynamicsWorld;

  class PhysicsSystem {
    public:
      PhysicsSystem();

      PhysicsSystem(glm::vec3 gravity);

      auto GetGravity();

      auto SetGravity(glm::vec3 gravity);

      auto update(entt::registry* registry, float dt) -> void;

    private:
      World* world;

      glm::vec3 gravity_;

    friend class Collision;
  };
};
