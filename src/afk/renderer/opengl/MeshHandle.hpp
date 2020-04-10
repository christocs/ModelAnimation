#pragma once

#include <cstddef>
#include <typeindex>
#include <typeinfo>
#include <vector>

#include <ctti/type_id.hpp>
#include <frozen/unordered_map.h>
#include <glad/glad.h>

#include "afk/physics/Transform.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/opengl/TextureHandle.hpp"
#include "afk/utility/ArrayOf.hpp"

namespace Afk {
  namespace OpenGl {
    struct IndexHash {
      constexpr auto operator()(const ctti::type_id_t &value, std::size_t seed) const
          -> std::size_t {
        return seed ^ value.hash();
      }
    };

    struct MeshHandle {
      using Textures = std::vector<TextureHandle>;

      static constexpr auto GL_INDICES =
          frozen::unordered_map<ctti::type_id_t, GLenum, 6, IndexHash>(
              {{ctti::type_id<signed char>(), GL_BYTE},
               {ctti::type_id<unsigned char>(), GL_UNSIGNED_BYTE},
               {ctti::type_id<short>(), GL_SHORT},
               {ctti::type_id<unsigned short>(), GL_UNSIGNED_SHORT},
               {ctti::type_id<int>(), GL_INT},
               {ctti::type_id<unsigned>(), GL_UNSIGNED_INT}});

      static constexpr auto INDEX = GL_INDICES.at(ctti::type_id<Mesh::Index>());

      GLuint vao              = {};
      GLuint vbo              = {};
      GLuint ibo              = {};
      Textures textures       = {};
      std::size_t num_indices = {};

      Transform transform = {};
    };
  }
}
