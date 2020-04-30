#pragma once

#include "afk/renderer/Model.hpp"

namespace Afk {
  class TerrainManager {
  public:
    TerrainManager()                       = default;
    TerrainManager(TerrainManager &&)      = delete;
    TerrainManager(const TerrainManager &) = delete;
    auto operator=(const TerrainManager &) -> TerrainManager & = delete;
    auto operator=(TerrainManager &&) -> TerrainManager & = delete;

    auto initialize() -> void;

  private:
    Mesh height = {};
    Mesh mesh   = {};

    bool is_initialized = false;

    auto generate_flat_plane(int width, int length) -> void;
    auto generate_height_map(int width, int length, float roughness, float scaling) -> void;
  };
}
