#pragma once

#include <vector>

#include "afk/physics/shape/HeightMap.hpp"
#include "afk/renderer/Model.hpp"

namespace Afk {
  class TerrainManager {
  public:
    HeightMap height_map = {};
    Mesh mesh            = {};

    TerrainManager(std::string filename);
    TerrainManager() = delete;

    auto initialize() -> void;
    auto get_model() -> Afk::Model;
    auto generate_terrain(int width, int length, float roughness, float scaling) -> void;

  private:
    bool is_initialized = false;

    std::string filename_;

    auto generate_flat_plane(int width, int length) -> void;
    auto generate_height_map(int width, int length, float roughness, float scaling) -> void;
  };
}
