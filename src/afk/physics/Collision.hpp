#pragma once

#include "afk/physics/Transform.hpp"
#include "afk/physics/shape/Box.hpp"
#include "afk/physics/shape/Sphere.hpp"
#include <reactphysics3d.h>
#include <entt/entt.hpp>
#include "glm/vec3.hpp"
#include <memory>


namespace Afk {
  class Collision {
    public:
      Collision() = delete;

      Collision(rp3d::DynamicsWorld* world, Afk::Transform transform, float mass, bool gravity, rp3d::BodyType bodyType, Afk::Shape::Box boundingBox);

      Collision(rp3d::DynamicsWorld* world, Afk::Transform transform, float mass, bool gravity, rp3d::BodyType bodyType, Afk::Shape::Sphere boundingSphere);
  
      rp3d::RigidBody* GetBody();
    private:
      rp3d::RigidBody* body;
      rp3d::ProxyShape* proxyShape;
      std::unique_ptr<rp3d::CollisionShape> collisionShape;
  };
}
