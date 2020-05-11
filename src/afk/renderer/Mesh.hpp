#pragma once

#include <cstdint>
#include <glob.h>
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

    glm::uvec4 bone_ids                  = glm::uvec4(0);
    glm::vec4 bone_weights               = glm::vec4(0.0f);
    int no_bones                         = 0;
    static constexpr const int MAX_BONES = 4;

    auto add_bone(unsigned int id, float weight) -> void;
  };

  struct Mesh {
    using Vertices = std::vector<Vertex>;
    using Index    = uint32_t;
    using Indices  = std::vector<Index>;
    using Textures = std::vector<Texture>;

    Vertices vertices = {};
    Indices indices   = {};
    Textures textures = {};

    size_t node_id = 0;
  };
}
