#include "AssetFactory.hpp"

#include <fstream>
#include <istream>
#include <map>
#include <string>
#include <vector>

#include "afk/debug/Assert.hpp"
#include "afk/io/ModelSource.hpp"
#include "afk/io/Path.hpp"
#include "afk/physics/PhysicsBody.hpp"
#include "afk/physics/RigidBodyType.hpp"
#include "afk/physics/Transform.hpp"

// nomove
#include "afk/script/LuaInclude.hpp"
// nomove
#include <LuaBridge/LuaBridge.h>

#include "afk/component/ScriptsComponent.hpp"

using Afk::Asset::Asset;
using namespace std::string_literals;
enum class Shape { Box, Sphere };

static auto load_script(lua_State *lua, LuaRef tbl) -> Afk::ScriptsComponent {
  auto script = Afk::ScriptsComponent{};
  for (int i = 1; i <= tbl.length(); i++) {
    script.add_script(tbl[i], lua, &Afk::Engine::get().event_manager);
  }
  return script;
}

static auto load_object_asset(lua_State *lua) -> Asset {
  auto obj        = Asset::Object{};
  auto &reg       = Afk::Engine::get().registry;
  obj.ent         = reg.create();
  auto components = LuaRef{luabridge::getGlobal(lua, "components")};
  afk_assert(components.isTable(), "components must be a table");
  auto tf = LuaRef{components["transform"]};
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
  auto phys = LuaRef{components["phys"]};
  if (!phys.isNil()) {

    // bounciness = 0.1, linear_dampening = 0.4, angular_dampening = 0.4,
    // mass = 0.5, gravity = true, body_type = rigidbody.dynamic,
    // shape = {type = shape.box}
    auto shape      = LuaRef{phys["shape"]};
    auto shape_type = shape["type"].cast<Shape>();
    switch (shape_type) {
      case Shape::Box: {
        auto box = Afk::Box{shape["x"].cast<float>(), shape["y"].cast<float>(),
                            shape["z"].cast<float>()};
        reg.assign<Afk::PhysicsBody>(
            obj.ent,
            Afk::PhysicsBody{&Afk::Engine::get().physics_body_system,
                             reg.get<Afk::Transform>(obj.ent),
                             phys["bounciness"].cast<float>(),
                             phys["linear_damping"].cast<float>(),
                             phys["angular_damping"].cast<float>(),
                             phys["mass"].cast<float>(), phys["gravity"].cast<bool>(),
                             phys["body_type"].cast<Afk::RigidBodyType>(), box});
        break;
      }
      case Shape::Sphere: {

        auto sphere = Afk::Sphere{shape["r"].cast<float>()};
        reg.assign<Afk::PhysicsBody>(
            obj.ent,
            Afk::PhysicsBody{&Afk::Engine::get().physics_body_system,
                             reg.get<Afk::Transform>(obj.ent),
                             phys["bounciness"].cast<float>(),
                             phys["linear_damping"].cast<float>(),
                             phys["angular_damping"].cast<float>(),
                             phys["mass"].cast<float>(), phys["gravity"].cast<bool>(),
                             phys["body_type"].cast<Afk::RigidBodyType>(), sphere});
        break;
      }
    }
  } // phys
  auto mdl = LuaRef{components["model"]};
  if (!mdl.isNil()) {
    reg.assign<Afk::ModelSource>(obj.ent,
                                 Afk::ModelSource{mdl["path"].cast<std::string>()});
  }
  auto script = LuaRef{components["script"]};
  if (!script.isNil()) {
    reg.assign<Afk::ScriptsComponent>(obj.ent, load_script(lua, script));
  }

  return Afk::Asset::Asset{Afk::Asset::Asset::AssetData{obj}, Afk::Asset::AssetType::Object};
}
static auto load_terrain_asset([[maybe_unused]] lua_State *lua) -> Asset {
  // todo: implement
  throw std::runtime_error{"unimplemented"};
}

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
  rigidbody_enum.addVariable("dynamic", const_cast<int *>(&DYNAMIC), false);
  rigidbody_enum.addVariable("kinematic", const_cast<int *>(&KINEMATIC), false);
  rigidbody_enum.addVariable("static", const_cast<int *>(&STATIC), false);
  rigidbody_enum.endNamespace();
  auto shape_enum = luabridge::getGlobalNamespace(lua).beginNamespace("shape");
  constexpr auto BOX    = static_cast<int>(Shape::Box);
  constexpr auto SPHERE = static_cast<int>(Shape::Sphere);
  asset_namespace.addVariable("box", const_cast<int *>(&BOX), false);
  asset_namespace.addVariable("sphere", const_cast<int *>(&SPHERE), false);
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
    case AssetType::Object: a = load_object_asset(lua); break;
    case AssetType::Terrain: a = load_terrain_asset(lua); break;
  }
  lua_close(lua);
  return a;
}
