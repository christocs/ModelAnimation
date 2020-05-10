#pragma once

#include <vector>

#include "afk/physics/Transform.hpp"
#include "afk/renderer/Animation.hpp"
#include "afk/renderer/ModelNode.hpp"
#include "afk/renderer/Renderer.hpp"

namespace Afk {
  namespace OpenGl {
    struct ModelHandle {
      using Meshes     = std::vector<MeshHandle>;
      using Nodes      = std::vector<ModelNode>;
      // TODO: convert to use frozen unordered_map for better efficiency
      using Animations = std::unordered_map<std::string, Animation>;

      size_t root_node_index = 0;

      Nodes nodes           = {};
      Meshes meshes         = {};
      Animations animations = {};
    };
  }
}
