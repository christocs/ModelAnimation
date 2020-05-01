#pragma once

#include <string>

#include "afk/component/BaseComponent.hpp"

namespace Afk {
  class ModelSource : public BaseComponent {
  public:
    ModelSource(GameObject e, std::string name);
    std::string name;
  };
}
