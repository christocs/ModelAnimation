#pragma once

#include "afk/renderer/Model.hpp"

namespace Afk {
  class TerrainGenerator {
  public:
    auto generateFlatPlane(int size, int width) -> void;
    auto take_mesh() -> Mesh;

  private:
    Mesh mesh = {};
  };
}
