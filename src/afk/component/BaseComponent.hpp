#pragma once

#include <entt/entt.hpp>

#include "afk/ecs/GameObject.hpp"
namespace Afk {
  /**
   * Base class for all components
   */
  class BaseComponent {
  public:
    /**
     * The entity that owns this component. Ideally, set this
     * in a constructor.
     */
    GameObject owning_entity = {entt::null};
  };
}
