#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "afk/physics/Transform.hpp"
#include "afk/render/TextureData.hpp"

namespace Afk {
  struct VertexData {
    glm::vec3 position  = {};
    glm::vec3 normal    = {};
    glm::vec2 uvs       = {};
    glm::vec3 tangent   = {};
    glm::vec3 bitangent = {};
  };

  struct MeshData {
    using Vertices = std::vector<VertexData>;
    using Index    = unsigned;
    using Indices  = std::vector<Index>;
    using Textures = std::vector<TextureData>;

    Vertices vertices   = {};
    Indices indices     = {};
    Textures textures   = {};
    Transform transform = {};
  };
}
