#include "afk/physics/Collision.hpp"
#include <iostream>
using Afk::Collision;

Collision::Collision(rp3d::DynamicsWorld* world, Afk::Transform transform, float mass, bool gravity, rp3d::BodyType bodyType, Afk::Shape::Box boundingBox) {
  this->collisionShape = std::make_unique<rp3d::BoxShape>(rp3d::Vector3(
    boundingBox.x * transform.scale.x,
    boundingBox.y * transform.scale.y,
    boundingBox.z * transform.scale.z
  ));

  this->body = world->createRigidBody(rp3d::Transform(
    rp3d::Vector3(transform.translation[0], transform.translation[1], transform.translation[2]),
    rp3d::Quaternion(transform.rotation[0], transform.rotation[1], transform.rotation[2], transform.rotation[3])
  ));

  this->body->enableGravity(gravity);
  this->body->setType(bodyType);

  this->proxyShape = this->body->addCollisionShape(this->collisionShape.get(), rp3d::Transform::identity(), mass);
}

Collision::Collision(rp3d::DynamicsWorld* world, Afk::Transform transform, float mass, bool gravity, rp3d::BodyType bodyType, Afk::Shape::Sphere boundingSphere) {
  // Note: have to scale sphere equally on every axis (otherwise it wouldn't be a sphere), so scaling the average of each axis
  const auto scaleFactor = (transform.scale.x + transform.scale.y + transform.scale.z) / 3.0f;
  this->collisionShape = std::make_unique<rp3d::SphereShape>(boundingSphere * scaleFactor);

  this->body = world->createRigidBody(rp3d::Transform(
    rp3d::Vector3(transform.translation[0], transform.translation[1], transform.translation[2]),
    rp3d::Quaternion(transform.rotation[0], transform.rotation[1], transform.rotation[2], transform.rotation[3])
  ));

  this->body->enableGravity(gravity);
  this->body->setType(bodyType);

  this->proxyShape = this->body->addCollisionShape(this->collisionShape.get(), rp3d::Transform::identity(), mass);
}

rp3d::RigidBody* Collision::GetBody() {
  return this->body;
}
