#pragma once

#include "afk/renderer/Model.hpp"

namespace Afk {
  class TerrainGenerator {
  public:
    TerrainGenerator(int width, int length, float scaling = 10.0f);
    auto get_model() -> Model;

  private:
    Mesh height = {};
    Mesh mesh   = {};

    auto generate_flat_plane(int width, int length) -> void;
    auto generate_height_map(int width, int length, float scaling) -> void;
  };
}
