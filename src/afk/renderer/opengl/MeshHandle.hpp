#pragma once

#include <cstddef>
#include <vector>

#include <glad/glad.h>

#include "afk/physics/Transform.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/opengl/TextureHandle.hpp"

namespace Afk {
  namespace OpenGl {
    struct MeshHandle {
      using Textures = std::vector<TextureHandle>;

      GLuint vao              = {};
      GLuint vbo              = {};
      GLuint ibo              = {};
      Textures textures       = {};
      std::size_t num_indices = {};

      Transform transform = {};
    };
  }
}
