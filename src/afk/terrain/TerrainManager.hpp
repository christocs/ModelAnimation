#pragma once

#include <vector>

#include "afk/physics/shape/HeightMap.hpp"
#include "afk/renderer/Model.hpp"

namespace Afk {
  class TerrainManager {
  public:
    HeightMap height_map = {};
    Mesh mesh           = {};

    TerrainManager()                       = default;
    TerrainManager(TerrainManager &&)      = delete;
    TerrainManager(const TerrainManager &) = delete;
    auto operator=(const TerrainManager &) -> TerrainManager & = delete;
    auto operator=(TerrainManager &&) -> TerrainManager & = delete;

    auto initialize() -> void;
    auto get_model() -> Afk::Model;
    auto generate_terrain(int width, int length, float roughness, float scaling) -> void;

  private:
    bool is_initialized = false;

    auto generate_flat_plane(int width, int length) -> void;
    auto generate_height_map(int width, int length, float roughness, float scaling) -> void;
    auto centre_terrain() -> void;
  };
}
