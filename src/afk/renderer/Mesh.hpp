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
    static const size_t MAX_BONES                = 8;
    std::array<unsigned int, MAX_BONES> bone_ids = {};
    std::array<float, MAX_BONES> bone_weights    = {};

    auto add_bone(unsigned int id, float weight) -> void;
  };

  struct Mesh {
    using Vertices = std::vector<Vertex>;
    using Index    = uint32_t;
    using Indices  = std::vector<Index>;
    using Textures = std::vector<Texture>;

    Vertices vertices          = {};
    Indices indices            = {};
    Textures textures          = {};
    Transform transform        = {};
    Transform offset_transform = {};
    std::string node_name;
  };
}
