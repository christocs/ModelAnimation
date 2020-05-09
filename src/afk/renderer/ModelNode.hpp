#pragma once

#include "afk/physics/Transform.hpp"
#include "afk/renderer/Mesh.hpp"

namespace Afk {
  struct ModelNode {
    using Id       = size_t;
    using ChildIds = std::vector<Id>;
    using MeshIds  = std::vector<Id>;

    std::string name;
    // points to index of child nodes
    ChildIds childrenIds = {};
    // points to index of meshes contained in node
    MeshIds meshIds      = {};
    Transform transform  = {};
  };
}
