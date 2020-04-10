#include "afk/physics/Collision.hpp"

using Afk::Collision;

Collision::Collision(rp3d::DynamicsWorld* world, glm::vec3 boundingBox, Afk::Transform transform, float mass, bool gravity, rp3d::BodyType bodyType)
{
  // Convert Afk transform object to rp3d transform object
  // Note that Afk::Transform stores scale and rp3d::Transform does not
  auto transform_ = rp3d::Transform(
    rp3d::Vector3(transform.translation[0], transform.translation[1], transform.translation[2]),
    rp3d::Quaternion(transform.rotation[0], transform.rotation[1], transform.rotation[2], transform.rotation[3])
  );

  this->body = world->createRigidBody(transform_);

  this->body->enableGravity(gravity);
  this->body->setType(bodyType);

  this->collisionShape = new rp3d::BoxShape(rp3d::Vector3(boundingBox.x, boundingBox.y, boundingBox.z));

  this->proxyShape = this->body->addCollisionShape(this->collisionShape, rp3d::Transform::identity(), mass);
}

rp3d::RigidBody* Collision::GetBody()
{
  return this->body;
}

/*
Collision::~Collision()
{
  delete this->proxyShape;
  delete this->collisionShape;
  delete this->body;
}*/
