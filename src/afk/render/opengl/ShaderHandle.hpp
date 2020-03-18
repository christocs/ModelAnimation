#pragma once

#include <glad/glad.h>

#include "afk/render/ShaderData.hpp"

namespace Afk {
  namespace OpenGl {
    struct ShaderHandle {
      using Type = ShaderData::Type;

      GLuint id = {};
      Type type = {};
    };
  }
}
