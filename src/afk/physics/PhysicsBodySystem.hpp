#pragma once

#include <memory>
#include <reactphysics3d.h>

#include <entt/entt.hpp>

#include "afk/physics/PhysicsBody.hpp"
#include "glm/vec3.hpp"

namespace Afk {
  class PhysicsBody;

  using World = std::unique_ptr<rp3d::DynamicsWorld>;

  /**
   * physics body system
   */
  class PhysicsBodySystem {
  public:
    /**
     * constructor
     */
    PhysicsBodySystem();
    /**
     * constructor, with gravity
     */
    explicit PhysicsBodySystem(glm::vec3 gravity);
    /**
     * get current graivty
     */
    auto get_gravity() -> glm::vec3;
    /**
     * set current gravity
     */
    auto set_gravity(glm::vec3 gravity);
    /**
     * update all physics components
     */
    auto update(entt::registry *registry, float dt) -> void;

  private:
    World world = nullptr;

    friend class PhysicsBody;
  };
};
