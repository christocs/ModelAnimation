#pragma once

#include <filesystem>
#include <vector>

#include <assimp/scene.h>

#include "afk/renderer/Animation.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/ModelNode.hpp"
#include "afk/renderer/Texture.hpp"

namespace Afk {
  struct Model {
    using Meshes     = std::vector<Mesh>;
    using Nodes      = std::vector<ModelNode>;
    using NodeMap    = std::unordered_map<std::string, unsigned int>;
    using Animations = std::unordered_map<std::string, Animation>;

    Nodes nodes           = {};
    NodeMap node_map      = {};
    Animations animations = {};
    Meshes meshes         = {};

    size_t root_node_index = 0;

    std::filesystem::path file_path = {};
    std::filesystem::path file_dir  = {};

    Model() = default;
    Model(const std::filesystem::path &_file_path);
  };
}
