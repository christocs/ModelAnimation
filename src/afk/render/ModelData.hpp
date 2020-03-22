#pragma once

#include <vector>

#include "afk/render/MeshData.hpp"
#include "afk/render/TextureData.hpp"

namespace Afk {
  struct ModelData {
    using Meshes = std::vector<MeshData>;

    Meshes meshes    = {};
    std::string path = {};
    std::string dir  = {};

    ModelData() = default;
    ModelData(const std::string &_path);
  };
}
