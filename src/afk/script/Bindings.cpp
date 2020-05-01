#include "Bindings.hpp"
extern "C" {
#include <lua/lauxlib.h>
#include <lua/lua.h>
#include <lua/lualib.h>
}
#include <LuaBridge/LuaBridge.h>

#include "afk/Afk.hpp"
#include "afk/component/BaseComponent.hpp"
#include "afk/component/ScriptsComponent.hpp"
#include "afk/ecs/GameObject.hpp"
#include "afk/io/ModelSource.hpp"
#include "afk/physics/PhysicsBody.hpp"
#include "afk/physics/Transform.hpp"
#include "afk/renderer/Camera.hpp"

static auto get_camera() -> Afk::Camera & {
  return Afk::Engine::get().camera;
}

static auto new_vec3(float x, float y, float z) -> glm::vec3 {
  return glm::vec3{x, y, z};
}
static auto new_vec2(float x, float y) -> glm::vec2 {
  return glm::vec2{x, y};
}
static auto new_quat(float x, float y, float z, float w) -> glm::quat {
  return glm::quat{x, y, z, w};
}
static auto get_delta_time() -> float {
  return Afk::Engine::get().get_delta_time();
}
template<typename T>
static auto get_component(Afk::GameObject o) -> T * {
  return Afk::Engine::get().registry.try_get<T>(o)
}

using namespace luabridge;
auto Afk::add_engine_bindings(lua_State *lua) {
  getGlobalNamespace(lua)
      .beginClass<glm::vec3>("vector3")
      .addStaticFunction("new", &new_vec3)
      .addProperty("x", &glm::vec3::x)
      .addProperty("y", &glm::vec3::y)
      .addProperty("z", &glm::vec3::z)
      .endClass()

      .beginClass<glm::vec2>("vector2")
      .addStaticFunction("new", &new_vec2)
      .addProperty("x", &glm::vec2::x)
      .addProperty("y", &glm::vec2::y)
      .endClass()

      .beginClass<glm::quat>("quaternion")
      .addStaticFunction("new", &new_quat)
      .addProperty("x", &glm::quat::x)
      .addProperty("y", &glm::quat::y)
      .addProperty("z", &glm::quat::y)
      .addProperty("w", &glm::quat::w)
      .endClass()

      .beginClass<Afk::Camera>("camera")
      .addStaticFunction("current", &get_camera)
      .addFunction("get_pos", &Afk::Camera::get_position)
      .addFunction("set_pos", &Afk::Camera::set_position)
      .addFunction("get_angle", &Afk::Camera::get_angles)
      .addFunction("set_angle", &Afk::Camera::set_angles)
      .endClass()

      .beginClass<Afk::GameObject>("entity")
      .addFunction("get_transform", &get_component<Transform>)
      .addFunction("get_physics", &get_component<PhysicsBody>)
      .addFunction("get_model", &get_component<ModelSource>)
      .addFunction("get_script", &get_component<ScriptsComponent>)
      .endClass()

      .beginClass<Afk::BaseComponent>("component")
      .addData("parent", &Afk::BaseComponent::owning_entity, false)
      .endClass()

      .beginClass<Afk::PhysicsBody>("physics_component")
      .addData("parent", &Afk::BaseComponent::owning_entity, false)
      .addFunction("apply_force", &Afk::PhysicsBody::apply_force)
      .addFunction("apply_torque", &Afk::PhysicsBody::apply_torque)
      .addFunction("translate", &Afk::PhysicsBody::translate)
      .endClass()

      .beginClass<Afk::Transform>("transform_component")
      .addData("parent", &Afk::BaseComponent::owning_entity, false)
      .addData("translation", &Afk::Transform::translation)
      .addData("rotation", &Afk::Transform::rotation)
      .addData("scale", &Afk::Transform::scale)
      .endClass()

      .beginClass<Afk::ModelSource>("model_component")
      .addData("parent", &Afk::BaseComponent::owning_entity, false)
      .addData("name", &Afk::ModelSource::name)
      .endClass()

      .beginNamespace("engine")
      .addFunction("delta_time", &get_delta_time)
      .endNamespace();
}
