#pragma once

#include <memory>
#include <reactphysics3d.h>

#include <entt/entt.hpp>

#include "afk/component/BaseComponent.hpp"
#include "afk/physics/PhysicsBodySystem.hpp"
#include "afk/physics/RigidBodyType.hpp"
#include "afk/physics/Transform.hpp"
#include "afk/physics/shape/Box.hpp"
#include "afk/physics/shape/HeightMap.hpp"
#include "afk/physics/shape/Sphere.hpp"
#include "glm/vec3.hpp"

namespace Afk {
  class PhysicsBodySystem;
  /**
   * rigidbody façade
   */
  using RigidBody = rp3d::RigidBody;
  /**
   * proxyshape façade
   */
  using ProxyShape = rp3d::ProxyShape;
  /**
   * collision shape façade
   */
  using CollisionShape = rp3d::CollisionShape;

  /**
   * physics component
   */
  class PhysicsBody : public BaseComponent {
  public:
    PhysicsBody() = delete;
    /**
     * construct a physics body with a box
     * @todo write @params
     */
    PhysicsBody(GameObject e, Afk::PhysicsBodySystem *physics_system,
                Afk::Transform transform, float bounciness, float linear_dampening,
                float angular_dampening, float mass, bool gravity_enabled,
                Afk::RigidBodyType body_type, Afk::Box bounding_box);

    /**
     * construct a physics body with a sphere
     * @todo write @params
     */
    PhysicsBody(GameObject e, Afk::PhysicsBodySystem *physics_system,
                Afk::Transform transform, float bounciness, float linear_dampening,
                float angular_dampening, float mass, bool gravity_enabled,
                Afk::RigidBodyType body_type, Afk::Sphere bounding_sphere);

    /**
     * construct a physics body with a heightmap
     * @todo write @params
     */
    PhysicsBody(GameObject e, Afk::PhysicsBodySystem *physics_system,
                Afk::Transform transform, float bounciness, float linear_dampening,
                float angular_dampening, float mass, bool gravity_enabled,
                Afk::RigidBodyType body_type, const Afk::HeightMap &height_map);

    // todo add rotate method

    /**
     *  translate the position of the physics body
     */
    void translate(glm::vec3 translate);

    /**
     * apply force to the centre of mass of the body for the next physics update
     */
    void apply_force(glm::vec3 force);

    /**
     * apply torque for the next physics update
     */
    void apply_torque(glm::vec3 force);

  private:
    RigidBody *body;
    ProxyShape *proxy_shape;
    std::unique_ptr<CollisionShape> collision_shape;

    friend class PhysicsBodySystem;
  };
}
