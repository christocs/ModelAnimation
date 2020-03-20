#pragma once

#include <glad/glad.h>

#include "afk/render/TextureData.hpp"

namespace Afk {
  namespace OpenGl {
    struct TextureHandle {
      using Type = TextureData::Type;

      Type type = {};
      GLuint id = {};
    };
  }
}
