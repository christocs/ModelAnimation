#pragma once

#include <filesystem>
#include <vector>

#include <assimp/scene.h>

#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/ModelNode.hpp"
#include "afk/renderer/Texture.hpp"

namespace Afk {
  struct Model {
    using Meshes = std::vector<Mesh>;
    using Nodes  = std::vector<ModelNode>;

    Nodes nodes   = {};
    Meshes meshes = {};

    size_t root_node_index = 0;

    std::filesystem::path file_path = {};
    std::filesystem::path file_dir  = {};

    //    struct Bone {
    //      std::string name;
    //      glm::mat4 offset_transform;
    //      glm::mat4 final_transform;
    //    };
    //
    //    using Bones = std::vector<Bone>;
    //    Bones bones = {};
    //    auto static find_bone_index(const std::string &name, const Bones &bones) -> int;

    Model() = default;
    Model(const std::filesystem::path &_file_path);
  };
}
