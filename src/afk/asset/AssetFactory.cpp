#include "AssetFactory.hpp"

#include <fstream>
#include <istream>
#include <map>
#include <string>
#include <vector>

#include "afk/debug/Assert.hpp"
#include "afk/io/Path.hpp"

// nomove
#include "afk/script/LuaInclude.hpp"
// nomove
#include <LuaBridge/LuaBridge.h>

using Afk::Asset::Asset;
using namespace std::string_literals;

struct RawAsset {
  using Values = std::vector<std::string>;
  std::filesystem::path asset_path;
  std::map<std::string, Values> kv_pairs;
};

auto load_object_asset(lua_State *lua) -> Asset {
  auto obj = Asset::Object{};
  // obj.ent = < CREATE ENTITY WITH ECS HERE >
  auto components = luabridge::getGlobal(lua, "components");
  afk_assert(components.isTable(), "components must be a table");
  // todo: write more when ECS is here.
}
auto load_terrain_asset(lua_State *lua) -> Asset {}

auto Afk::Asset::game_asset_factory(const std::filesystem::path &path) -> Asset {
  // For the purpose of simplicity and not having to write a new parser, we will simply
  // use a stripped down lua state (no opening libraries)
  // to load our game assets.
  lua_State *lua = luaL_newstate();
  auto asset_namespace =
      luabridge::getGlobalNamespace(lua).beginNamespace("asset");
  constexpr auto OBJECT  = static_cast<int>(AssetType::Object);
  constexpr auto TERRAIN = static_cast<int>(AssetType::Terrain);
  asset_namespace.addVariable("object", const_cast<int *>(&OBJECT), false);
  asset_namespace.addVariable("terrain", const_cast<int *>(&TERRAIN), false);
  asset_namespace.endNamespace();
  auto abs_path = Afk::get_absolute_path(path);
  afk_assert(luaL_dofile(lua, abs_path.c_str()) != 0,
             "Error loading "s + path.string() + ": "s + lua_tostring(lua, -1));
  const int asset_val   = luabridge::getGlobal(lua, "type");
  const auto asset_type = static_cast<AssetType>(asset_val);
  Asset a;
  switch (asset_type) {
    case AssetType::Object: a = load_object_asset(lua);
    case AssetType::Terrain: a = load_terrain_asset(lua);
  }
  lua_close(lua);
}
