#include "AssetFactory.hpp"

#include <fstream>
#include <istream>
#include <map>
#include <string>
#include <vector>

#include "afk/debug/Assert.hpp"
#include "afk/io/Path.hpp"
#include "afk/physics/Transform.hpp"

// nomove
#include "afk/script/LuaInclude.hpp"
// nomove
#include <LuaBridge/LuaBridge.h>

#include "afk/component/ScriptsComponent.hpp"

using Afk::Asset::Asset;
using namespace std::string_literals;

auto load_script(lua_State *lua, LuaRef tbl) -> Afk::ScriptsComponent {
  auto script = Afk::ScriptsComponent{};
  for (int i = 0; i < tbl.length; i++) {
    script.add_script(tbl[i], lua, &Afk::Engine::get().event_manager);
  }
  return script;
}

auto load_object_asset(lua_State *lua) -> Asset {
  auto obj        = Asset::Object{};
  auto &reg       = Afk::Engine::get().registry;
  obj.ent         = reg.create();
  auto components = luabridge::getGlobal(lua, "components");
  afk_assert(components.isTable(), "components must be a table");
  auto tf = components["transform"];
  if (!tf.isNil()) {
    auto transform          = Afk::Transform{};
    transform.translation.x = tf["x"];
    transform.translation.y = tf["y"];
    transform.translation.z = tf["z"];
    reg.assign<Afk::Transform>(obj.ent, transform);
  } else {
    auto transform = Afk::Transform{};
    reg.assign<Afk::Transform>(obj.ent, transform);
  }
  auto script = components["script"];
  if (!script.isNil()) {
    reg.assign<Afk::ScriptsComponent>(obj.ent, load_script(lua, script));
  }
  auto phys = components["phys"];
  if (!phys.isNil()) {
    // reg.assign<Afk::ScriptsComponent>(obj.ent, load_script(lua, script));
  }
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
  auto rigidbody_enum =
      luabridge::getGlobalNamespace(lua).beginNamespace("rigidbody");
  constexpr auto DYNAMIC   = static_cast<int>(RigidBodyType::DYNAMIC);
  constexpr auto KINEMATIC = static_cast<int>(RigidBodyType::KINEMATIC);
  constexpr auto STATIC    = static_cast<int>(RigidBodyType::STATIC);
  asset_namespace.addVariable("dynamic", const_cast<int *>(&DYNAMIC), false);
  asset_namespace.addVariable("kinematic", const_cast<int *>(&KINEMATIC), false);
  asset_namespace.addVariable("static", const_cast<int *>(&STATIC), false);
  asset_namespace.endNamespace();
  auto abs_path   = Afk::get_absolute_path(path);
  auto error_code = luaL_dofile(lua, abs_path.c_str());
  if (error_code != 0) {
    throw std::runtime_error{"Error loading "s + path.string() + ": "s +
                             lua_tostring(lua, -1)};
  }
  const int asset_val   = luabridge::getGlobal(lua, "type");
  const auto asset_type = static_cast<AssetType>(asset_val);
  Asset a;
  switch (asset_type) {
    case AssetType::Object: a = load_object_asset(lua);
    case AssetType::Terrain: a = load_terrain_asset(lua);
  }
  lua_close(lua);
}
