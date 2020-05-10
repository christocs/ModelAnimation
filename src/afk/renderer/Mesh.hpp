#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "afk/physics/Transform.hpp"
#include "afk/renderer/Texture.hpp"

namespace Afk {
  struct Vertex {
    glm::vec3 position  = {};
    glm::vec3 normal    = {};
    glm::vec2 uvs       = {};
    glm::vec3 tangent   = {};
    glm::vec3 bitangent = {};

    unsigned int no_bones                        = 0;
    static const unsigned int MAX_BONES          = 8;
    std::array<unsigned int, MAX_BONES> bone_ids = {};
    std::array<float, MAX_BONES> bone_weights    = {};

    auto add_bone(unsigned int id, float weight) -> void;
  };

  struct Bone {
    glm::mat4 offset_transform = glm::mat4(1.0f);
    glm::mat4 final_transform = glm::mat4(1.0f);
  };

  struct Mesh {
    using Vertices = std::vector<Vertex>;
    using Index    = uint32_t;
    using Indices  = std::vector<Index>;
    using Textures = std::vector<Texture>;
    using Bones    = std::vector<Bone>;
    using BoneMap  = std::unordered_map<std::string, size_t>;

    Vertices vertices = {};
    Indices indices   = {};
    Textures textures = {};
    Bones bones       = {};
    BoneMap bone_map  = {};
    std::string node_name;
  };
}
