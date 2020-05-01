#pragma once

#include <memory>
#include <reactphysics3d.h>

#include <entt/entt.hpp>

#include "afk/physics/PhysicsBody.hpp"
#include "glm/vec3.hpp"

namespace Afk {
  class PhysicsBody;

  using World = std::unique_ptr<rp3d::DynamicsWorld>;

  class PhysicsBodySystem {
  public:
    PhysicsBodySystem();

    explicit PhysicsBodySystem(glm::vec3 gravity);

    auto get_gravity();

    auto set_gravity(glm::vec3 gravity);

    auto update(entt::registry *registry, float dt) -> void;

  private:
    World world = nullptr;

    friend class PhysicsBody;
  };
};
