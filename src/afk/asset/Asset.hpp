#pragma once

#include <variant>

#include <entt/entt.hpp>

namespace Afk::Asset {
  /**
   * Asset type
   */
  enum class AssetType { Terrain, Object };
  /**
   * Asset loaded by assetfactory
   * @see AssetFactory
   */
  struct Asset {
    /**
     * Entity asset
     */
    struct Object {
      entt::entity ent;
    };
    /**
     * [WIP] Terrain asset
     */
    struct Terrain {};
    /**
     * Variant type for asset
     */
    typedef std::variant<std::monostate, Asset::Object, Asset::Terrain> AssetData;
    /**
     * Actual asset data variant
     */
    AssetData data;
    /**
     * The type of asset this is
     */
    AssetType asset_type;
  };
}
