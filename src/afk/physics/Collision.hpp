#pragma once

#include "afk/physics/PhysicsSystem.hpp"
#include "afk/physics/RigidBodyType.hpp"
#include "afk/physics/Transform.hpp"
#include "afk/physics/shape/Box.hpp"
#include "afk/physics/shape/Sphere.hpp"
#include <reactphysics3d.h>
#include <entt/entt.hpp>
#include "glm/vec3.hpp"
#include <memory>


namespace Afk {
  class PhysicsSystem;

  using RigidBody = rp3d::RigidBody;

  using ProxyShape = rp3d::ProxyShape;

  using CollisionShape = rp3d::CollisionShape;

  class Collision {
    public:
      Collision() = delete;

      Collision(Afk::PhysicsSystem* physics_system, Afk::Transform transform, float mass, bool gravity_enabled, Afk::RigidBodyType body_type, Afk::Box bounding_box);

      Collision(Afk::PhysicsSystem* physics_system, Afk::Transform transform, float mass, bool gravity_enabled, Afk::RigidBodyType body_type, Afk::Sphere bounding_sphere);

    private:
      RigidBody* body;
      ProxyShape* proxy_shape;
      std::unique_ptr<CollisionShape> collision_shape;

    friend class PhysicsSystem;
  };
}
