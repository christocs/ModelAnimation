#pragma once

#include <filesystem>
#include <string>

namespace Afk {
  struct Shader {
    enum class Type { Vertex, Fragment };

    std::filesystem::path file_path = {};
    std::string code                = {};
    Type type                       = {};

    Shader() = default;
    Shader(std::filesystem::path _file_path);
  };
}
