#pragma once

#include <filesystem>
#include <vector>

#include "afk/renderer/Shader.hpp"

namespace Afk {
  struct ShaderProgram {
    using ShaderPaths = std::vector<std::filesystem::path>;

    ShaderProgram() = default;
    ShaderProgram(std::filesystem::path _file_path);

    ShaderPaths shader_paths        = {};
    std::filesystem::path file_path = {};
  };
}
