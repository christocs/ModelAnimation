#include "afk/physics/PhysicsBodySystem.hpp"

#include "afk/physics/PhysicsBody.hpp"
#include "afk/physics/Transform.hpp"

using Afk::PhysicsBodySystem;

PhysicsBodySystem::PhysicsBodySystem() {
  this->gravity_ = glm::vec3(0.0f, -9.81f, 0.0f);
  this->world = new rp3d::DynamicsWorld(rp3d::Vector3(this->gravity_.x, this->gravity_.y, this->gravity_.z));
}

PhysicsBodySystem::PhysicsBodySystem(glm::vec3 gravity) {
  this->gravity_ = gravity;
  this->world = new rp3d::DynamicsWorld(rp3d::Vector3(this->gravity_.x, this->gravity_.y, this->gravity_.z));
}

auto PhysicsBodySystem::GetGravity() {
  return this->gravity_;
}

auto PhysicsBodySystem::SetGravity(glm::vec3 gravity) {
  this->gravity_ = gravity;
  auto rp3d_gravity = rp3d::Vector3(this->gravity_.x, this->gravity_.y, this->gravity_.z);
  this->world->setGravity(rp3d_gravity);
}

auto PhysicsBodySystem::update(entt::registry* registry, float dt) -> void {
  this->world->update(dt);

  // TODO: Scale shapes of rigid bodies on the fly
  // @see https://github.com/DanielChappuis/reactphysics3d/issues/103
  registry->view<Afk::Transform, Afk::PhysicsBody>().each([](Afk::Transform& transform, Afk::PhysicsBody & collision) {
      const auto rp3dPosition = collision.body->getTransform().getPosition();
      const auto rp3dOreientation = collision.body->getTransform().getOrientation();

      transform.translation = glm::vec3{rp3dPosition.x, rp3dPosition.y, rp3dPosition.z};
      transform.rotation = glm::quat{rp3dOreientation.w, rp3dOreientation.x, rp3dOreientation.y, rp3dOreientation.z};
  });
}
