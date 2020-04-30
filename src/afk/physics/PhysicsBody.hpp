#pragma once

#include <memory>
#include <reactphysics3d.h>

#include <entt/entt.hpp>

#include "afk/physics/PhysicsBodySystem.hpp"
#include "afk/physics/RigidBodyType.hpp"
#include "afk/physics/Transform.hpp"
#include "afk/physics/shape/Box.hpp"
#include "afk/physics/shape/Sphere.hpp"
#include "glm/vec3.hpp"

namespace Afk {
  class PhysicsBodySystem;

  using RigidBody = rp3d::RigidBody;

  using ProxyShape = rp3d::ProxyShape;

  using CollisionShape = rp3d::CollisionShape;

  class PhysicsBody {
  public:
    PhysicsBody() = delete;

    PhysicsBody(Afk::PhysicsBodySystem *physics_system,
                Afk::Transform transform, float mass, bool gravity_enabled,
                Afk::RigidBodyType body_type, Afk::Box bounding_box);

    PhysicsBody(Afk::PhysicsBodySystem *physics_system,
                Afk::Transform transform, float mass, bool gravity_enabled,
                Afk::RigidBodyType body_type, Afk::Sphere bounding_sphere);

    // todo add rotate method

    // translate the position of the physics body
    void translate(glm::vec3 translate);

    // apply force to the centre of mass of the body for the next physics update
    void apply_force(glm::vec3 force);

    // apply torque for the next physics update
    void apply_torque(glm::vec3 force);

  private:
    RigidBody *body;
    ProxyShape *proxy_shape;
    std::unique_ptr<CollisionShape> collision_shape;

    friend class PhysicsBodySystem;
  };
}
