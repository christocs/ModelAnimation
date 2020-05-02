#pragma once

namespace Afk {
  /**
   * the type of rigidbody
   * static: doesnt move
   * kinematic: moves manually
   * dynamic: moves through the physics system
   */
  enum class RigidBodyType { STATIC, KINEMATIC, DYNAMIC };
}
