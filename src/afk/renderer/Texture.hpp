#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace Afk {
  struct Texture {
    enum class Type : std::size_t {
      Diffuse = 0,
      Specular,
      Normal,
      Height,
      Count
    };

    Type type        = Type::Diffuse;
    std::string path = {};

    Texture() = default;
    Texture(const std::string &_path);
  };
}
