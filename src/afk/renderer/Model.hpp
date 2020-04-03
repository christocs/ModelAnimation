#pragma once

#include <vector>

#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/Texture.hpp"

namespace Afk {
  struct Model {
    using Meshes = std::vector<Mesh>;

    Meshes meshes    = {};
    std::string path = {};
    std::string dir  = {};

    Model() = default;
    Model(const std::string &_path);
  };
}
