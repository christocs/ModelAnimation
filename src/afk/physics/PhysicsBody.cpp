#include "afk/physics/PhysicsBody.hpp"

#include <iostream>

using Afk::PhysicsBody;

PhysicsBody::PhysicsBody(Afk::PhysicsBodySystem *physics_system,
                         Afk::Transform transform, float mass, bool gravity_enabled,
                         Afk::RigidBodyType body_type, Afk::Box bounding_box) {
  this->collision_shape = std::make_unique<rp3d::BoxShape>(rp3d::Vector3(
      bounding_box.x * transform.scale.x, bounding_box.y * transform.scale.y,
      bounding_box.z * transform.scale.z));

  this->body = physics_system->world->createRigidBody(rp3d::Transform(
      rp3d::Vector3(transform.translation[0], transform.translation[1],
                    transform.translation[2]),
      rp3d::Quaternion(transform.rotation[0], transform.rotation[1],
                       transform.rotation[2], transform.rotation[3])));

  this->body->enableGravity(gravity_enabled);

  switch (body_type) {
    case Afk::RigidBodyType::STATIC:
      this->body->setType(rp3d::BodyType::STATIC);
      break;
    case Afk::RigidBodyType::KINEMATIC:
      this->body->setType(rp3d::BodyType::KINEMATIC);
      break;
    case Afk::RigidBodyType::DYNAMIC:
      this->body->setType(rp3d::BodyType::DYNAMIC);
      break;
  }

  this->proxy_shape = this->body->addCollisionShape(this->collision_shape.get(),
                                                    rp3d::Transform::identity(), mass);
}

PhysicsBody::PhysicsBody(Afk::PhysicsBodySystem *physics_system,
                         Afk::Transform transform, float mass, bool gravity_enabled,
                         Afk::RigidBodyType body_type, Afk::Sphere bounding_sphere) {
  // Note: have to scale sphere equally on every axis (otherwise it wouldn't be a sphere), so scaling the average of each axis
  const auto scaleFactor =
      (transform.scale.x + transform.scale.y + transform.scale.z) / 3.0f;
  this->collision_shape = std::make_unique<rp3d::SphereShape>(bounding_sphere * scaleFactor);

  this->body = physics_system->world->createRigidBody(rp3d::Transform(
      rp3d::Vector3(transform.translation[0], transform.translation[1],
                    transform.translation[2]),
      rp3d::Quaternion(transform.rotation[0], transform.rotation[1],
                       transform.rotation[2], transform.rotation[3])));

  this->body->enableGravity(gravity_enabled);

  switch (body_type) {
    case Afk::RigidBodyType::STATIC:
      this->body->setType(rp3d::BodyType::STATIC);
      break;
    case Afk::RigidBodyType::KINEMATIC:
      this->body->setType(rp3d::BodyType::KINEMATIC);
      break;
    case Afk::RigidBodyType::DYNAMIC:
      this->body->setType(rp3d::BodyType::DYNAMIC);
      break;
  }

  this->proxy_shape = this->body->addCollisionShape(this->collision_shape.get(),
                                                    rp3d::Transform::identity(), mass);
}

void PhysicsBody::translate(glm::vec3 translate) {
  this->body->setTransform(rp3d::Transform{
      rp3d::Vector3{this->body->getTransform().getPosition().x + translate.x,
                    this->body->getTransform().getPosition().y + translate.y,
                    this->body->getTransform().getPosition().z + translate.z},
      this->body->getTransform().getOrientation()});
}

void PhysicsBody::apply_force(glm::vec3 force) {
  this->body->applyForceToCenterOfMass(rp3d::Vector3{force.x, force.y, force.z});
}

void PhysicsBody::apply_torque(glm::vec3 torque) {
  this->body->applyTorque(rp3d::Vector3{torque.x, torque.y, torque.z});
}
