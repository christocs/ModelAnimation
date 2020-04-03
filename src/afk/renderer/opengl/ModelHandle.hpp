#pragma once

#include <vector>

#include "afk/physics/Transform.hpp"
#include "afk/renderer/Renderer.hpp"

namespace Afk {
  namespace OpenGl {
    struct ModelHandle {
      using Meshes = std::vector<MeshHandle>;

      Meshes meshes = {};
    };
  }
};
