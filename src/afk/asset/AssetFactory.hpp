#pragma once

#include <filesystem>
#include <string>

#include "afk/asset/Asset.hpp"
#include "afk/script/LuaInclude.hpp"

namespace Afk::Asset {
  /**
   * Loads an asset
   * @param path Path of asset to load
   * @return the loaded asset
   */
  auto game_asset_factory(const std::filesystem::path &path) -> Asset;
}
