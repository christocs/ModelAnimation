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
    typedef std::variant<std::monostate, Asset::Object, Asset::Terrain> AssetData;
    AssetData data;
    AssetType asset_type;
  };
}
