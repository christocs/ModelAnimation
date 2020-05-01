#pragma once

#include <entt/entt.hpp>

#include "afk/ecs/GameObject.hpp"
namespace Afk {
  class BaseComponent {
  public:
    GameObject owning_entity = {entt::null};
  };
}
