#pragma once

#include <reactphysics3d.h>
#include <entt/entt.hpp>
#include "afk/physics/Transform.hpp"


namespace Afk {
  class Collision {
    public:
      Collision() = delete;

      Collision(rp3d::DynamicsWorld* world, rp3d::CollisionShape* shape, Afk::Transform transform, float mass, bool gravity, rp3d::BodyType bodyType);
    
      rp3d::RigidBody* GetBody();
    private:
      rp3d::RigidBody* body;
      rp3d::ProxyShape* proxyShape;
  };
}
