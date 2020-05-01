#pragma once

#include <vector>

#include "afk/renderer/Model.hpp"

namespace Afk {
  struct HeightMap {
    struct Point {
      int x = {};
      int y = {};
    };

    std::vector<float> heights = {};
    int width                  = {};

    auto at(Point p) const -> float;
    auto operator[](Point p) -> float &;
  };

  class TerrainManager {
  public:
    HeightMap heightMap = {};
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
  };
}
