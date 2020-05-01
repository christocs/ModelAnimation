#pragma once

#include "afk/component/BaseComponent.hpp"

#include <string>
#include <filesystem>

namespace Afk {
  class ModelSource : public BaseComponent {
  public:
    ModelSource(GameObject e, std::string name, std::filesystem::path shader_path);
    std::string name;
    std::filesystem::path shader_program_path;
  };
}
