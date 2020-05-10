#pragma once

#include "glm/mat4x4.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/physics/Transform.hpp"

namespace Afk {
  struct ModelNode {
    using Id       = size_t;
    using ChildIds = std::vector<Id>;
    using MeshIds  = std::vector<Id>;

    std::string name;
    // points to index of child nodes
    ChildIds child_ids = {};
    // points to index of meshes contained in node
    MeshIds mesh_ids     = {};
    Transform transform  = {};
  };
}
