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
  };

  struct Mesh {
    using Vertices = std::vector<Vertex>;
    using Index    = uint16_t;
    using Indices  = std::vector<Index>;
    using Textures = std::vector<Texture>;

    Vertices vertices   = {};
    Indices indices     = {};
    Textures textures   = {};
    Transform transform = {};
  };
}
