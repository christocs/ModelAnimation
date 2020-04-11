#pragma once

#include <variant>

#include <entt/entt.hpp>

namespace Afk::Asset {
  enum class AssetType { Terrain, Object };
  struct Asset {
    struct Object {
      entt::entity ent;
    };
    struct Terrain {};
    std::variant<Object, Terrain> data;
    AssetType asset_type;
  };
}
