#pragma once

#include <string>

namespace Afk {
  struct ShaderData {
    enum class Type { Vertex, Fragment };

    std::string path = {};
    std::string code = {};
    Type type        = {};

    ShaderData() = default;
    ShaderData(const std::string &_path);
  };
}
