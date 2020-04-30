#include "afk/physics/PhysicsSystem.hpp"

#include "afk/physics/Transform.hpp"
#include "afk/physics/Collision.hpp"

using Afk::PhysicsSystem;

PhysicsSystem::PhysicsSystem() {
  this->gravity_ = glm::vec3(0.0f, -9.81f, 0.0f);
  this->world = new rp3d::DynamicsWorld(rp3d::Vector3(this->gravity_.x, this->gravity_.y, this->gravity_.z));
}

PhysicsSystem::PhysicsSystem(glm::vec3 gravity) {
  this->gravity_ = gravity;
  this->world = new rp3d::DynamicsWorld(rp3d::Vector3(this->gravity_.x, this->gravity_.y, this->gravity_.z));
}

auto PhysicsSystem::GetGravity() {
  return this->gravity_;
}

auto PhysicsSystem::SetGravity(glm::vec3 gravity) {
  this->gravity_ = gravity;
  auto rp3d_gravity = rp3d::Vector3(this->gravity_.x, this->gravity_.y, this->gravity_.z);
  this->world->setGravity(rp3d_gravity);
}

auto PhysicsSystem::update(entt::registry* registry, float dt) -> void {
  this->world->update(dt);

  // TODO: Scale shapes of rigid bodies on the fly
  // @see https://github.com/DanielChappuis/reactphysics3d/issues/103
  registry->view<Afk::Transform, Afk::Collision>().each([](Afk::Transform& transform, Afk::Collision& collision) {
      const auto rp3dPosition = collision.body->getTransform().getPosition();
      const auto rp3dOreientation = collision.body->getTransform().getOrientation();

      transform.translation = glm::vec3{rp3dPosition.x, rp3dPosition.y, rp3dPosition.z};

      // TODO: Fix rotation flipping the city upside down
      //transform.rotation = glm::quat{rp3dOreientation.x, rp3dOreientation.y, rp3dOreientation.z, rp3dOreientation.w};
  });
}
