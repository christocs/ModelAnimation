#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <SFML/Graphics/Image.hpp>

namespace Afk {
  struct TextureData {
    enum class Type : std::size_t {
      Diffuse = 0,
      Specular,
      Normal,
      Height,
    };

    Type type        = Type::Diffuse;
    std::string path = {};
    sf::Image image  = {};

    TextureData() = default;
    TextureData(const std::string &_path);
  };
}
