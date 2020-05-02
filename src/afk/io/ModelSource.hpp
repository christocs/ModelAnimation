#pragma once

#include <filesystem>
#include <string>

#include "afk/component/BaseComponent.hpp"

namespace Afk {
  /**
   * model component
   * @todo rename to model component
   */
  class ModelSource : public BaseComponent {
  public:
    /**
     * constructor
     * @param e owner
     * @param name_ model path
     * @param shader_path shader path
     */
    ModelSource(GameObject e, const std::filesystem::path &name_,
                const std::filesystem::path &shader_path);
    std::filesystem::path name;
    std::filesystem::path shader_program_path;
  };
}
