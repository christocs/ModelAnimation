#pragma once

#include <vector>

#include "afk/physics/Transform.hpp"
#include "afk/render/Renderer.hpp"

namespace Afk {
  namespace OpenGl {
    struct ModelHandle {
      using Meshes = std::vector<MeshHandle>;

      Meshes meshes       = {};
      Transform transform = {};
    };
  }
};
