#pragma once

#include <filesystem>
#include <vector>

#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/Texture.hpp"

namespace Afk {
  /**
   * model
   */
  struct Model {
    using Meshes = std::vector<Mesh>;

    Meshes meshes = {};

    std::filesystem::path file_path = {};
    std::filesystem::path file_dir  = {};

    Model() = default;
    Model(const std::filesystem::path &_file_path);
  };
}
