#pragma once

#include "afk/component/BaseComponent.hpp"

#include <string>
#include <filesystem>

namespace Afk {
  class ModelSource : public BaseComponent {
  public:
    ModelSource(GameObject e, const std::filesystem::path &name_,
                const std::filesystem::path &shader_path);
    std::filesystem::path name;
    std::filesystem::path shader_program_path;
  };
}
