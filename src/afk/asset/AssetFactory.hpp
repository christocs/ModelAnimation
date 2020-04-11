#pragma once

#include <filesystem>
#include <string>

#include "afk/asset/Asset.hpp"
#include "afk/script/LuaInclude.hpp"

namespace Afk::Asset {
  auto game_asset_factory(const std::filesystem::path &path) -> Asset;
}
