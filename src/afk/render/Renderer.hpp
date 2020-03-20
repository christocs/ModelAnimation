#pragma once

// Renderer header
#include "afk/render/opengl/Renderer.hpp"
// Handle headers
#include "afk/render/opengl/MeshHandle.hpp"
#include "afk/render/opengl/ModelHandle.hpp"
#include "afk/render/opengl/ShaderHandle.hpp"
#include "afk/render/opengl/ShaderProgramHandle.hpp"
#include "afk/render/opengl/TextureHandle.hpp"

namespace Afk {
  namespace OpenGl {
    class Renderer;
  }

  using Renderer = OpenGl::Renderer;
}
