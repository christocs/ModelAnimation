#pragma once

#include <string>

namespace Afk {
  struct Shader {
    enum class Type { Vertex, Fragment };

    std::string path = {};
    std::string code = {};
    Type type        = {};

    Shader() = default;
    Shader(const std::string &_path);
  };
}
