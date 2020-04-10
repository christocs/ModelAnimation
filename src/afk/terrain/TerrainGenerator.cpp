#include "afk/terrain/TerrainGenerator.hpp"

#include <utility>

#include <glm/glm.hpp>

#include "afk/debug/Assert.hpp"
#include "afk/renderer/Mesh.hpp"

using glm::vec2;
using glm::vec3;

using Afk::Mesh;
using Afk::Model;
using Afk::TerrainGenerator;
using Index = Mesh::Index;

auto TerrainGenerator::generateFlatPlane(int size, int width) -> void {
  afk_assert(size >= 1, "Invalid size");
  afk_assert(width >= 1, "Invalid width");

  // Generate vertices.
  for (auto z = 0; z < size; ++z) {
    for (auto x = 0; x <= size; ++x) {
      auto vertex = Vertex{};
      vertex.position =
          vec3{static_cast<float>(x * width), 0.0f, static_cast<float>(z * width)};
      vertex.uvs = vec2{static_cast<float>(x) / static_cast<float>(size),
                        static_cast<float>(z) / static_cast<float>(size)};
      this->mesh.vertices.push_back(vertex);

      vertex.position = vec3{static_cast<float>(x * width), 0.0f,
                             static_cast<float>((z + 1) * width)};
      vertex.uvs      = vec2{static_cast<float>(x) / static_cast<float>(size),
                        static_cast<float>(z + 1) / static_cast<float>(size)};

      this->mesh.vertices.push_back(vertex);
    }
  }

  // Generate indices.
  for (auto z = 0; z < size; ++z) {
    for (auto x = 0; x < size; ++x) {
      auto base = x * 2 + z * (size + 1) * 2;

      // Bottom-left triangle
      this->mesh.indices.push_back(static_cast<Index>(base + 2)); // BR
      this->mesh.indices.push_back(static_cast<Index>(base + 1)); // TL
      this->mesh.indices.push_back(static_cast<Index>(base + 0)); // BL
      // Top-right triangle
      this->mesh.indices.push_back(static_cast<Index>(base + 2)); // BR
      this->mesh.indices.push_back(static_cast<Index>(base + 3)); // TR
      this->mesh.indices.push_back(static_cast<Index>(base + 1)); // TL
    }
  }
}

auto TerrainGenerator::take_mesh() -> Mesh {
  return std::move(this->mesh);
}
