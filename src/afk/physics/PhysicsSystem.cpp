#include "afk/physics/PhysicsSystem.hpp"

#include "afk/physics/Transform.hpp"
#include "afk/physics/Collision.hpp"

auto Afk::update_physics(entt::registry* registry, rp3d::DynamicsWorld* world, float dt) -> void {
  world->update(dt);

  // transfer transform
  registry->view<Afk::Transform, Afk::Collision>().each([](Afk::Transform& transform, Afk::Collision& collision)
  {
      const auto rp3dPosition = collision.GetBody()->getTransform().getPosition();
      const auto rp3dOreientation = collision.GetBody()->getTransform().getOrientation();

      transform.translation = glm::vec3{rp3dPosition.x, rp3dPosition.y, rp3dPosition.z};

      // TODO: Fix rotation flipping the city upside down
      //transform.rotation = glm::quat{rp3dOreientation.x, rp3dOreientation.y, rp3dOreientation.z, rp3dOreientation.w};
  });
}
