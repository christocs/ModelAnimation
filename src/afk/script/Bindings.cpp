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

struct GameObjectWrapped {
  Afk::GameObject e;
  template<typename T>
  auto get_component() -> T * {
    return Afk::Engine::get().registry.try_get<T>(this->e);
  }
};

template<typename T>
static auto get_parent(T *bc) -> ENTT_ID_TYPE {
  return static_cast<ENTT_ID_TYPE>(bc->owning_entity);
}

static auto v3_add(glm::vec3 *a, glm::vec3 b) -> glm::vec3 {
  auto n = glm::vec3{*a};
  n += b;
  return n;
}
static auto v3_sub(glm::vec3 *a, glm::vec3 b) -> glm::vec3 {
  auto n = glm::vec3{*a};
  n -= b;
  return n;
}
static auto v3_mul(glm::vec3 *a, float f) -> glm::vec3 {
  auto n = glm::vec3{*a};
  n *= f;
  return n;
}

using namespace luabridge;
auto Afk::add_engine_bindings(lua_State *lua) {
  getGlobalNamespace(lua)
      .beginClass<glm::vec3>("vector3")
      .addStaticFunction("new", &new_vec3)
      .addData("x", &glm::vec3::x)
      .addData("y", &glm::vec3::y)
      .addData("z", &glm::vec3::z)
      .addFunction("vec_add", &v3_add)
      .addFunction("vec_sub", &v3_sub)
      .addFunction("sc_mul", &v3_mul)
      .endClass()

      .beginClass<glm::vec2>("vector2")
      .addStaticFunction("new", &new_vec2)
      .addData("x", &glm::vec2::x)
      .addData("y", &glm::vec2::y)
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
      .addFunction("front", &Afk::Camera::get_front)
      .addFunction("right", &Afk::Camera::get_right)
      .addFunction("up", &Afk::Camera::get_up)
      .endClass()

      .beginClass<GameObjectWrapped>("entity")
      .addFunction("get_transform", &GameObjectWrapped::get_component<Transform>)
      .addFunction("get_physics", &GameObjectWrapped::get_component<PhysicsBody>)
      .addFunction("get_model", &GameObjectWrapped::get_component<ModelSource>)
      .addFunction("get_script", &GameObjectWrapped::get_component<ScriptsComponent>)
      .endClass()

      .beginClass<Afk::PhysicsBody>("physics_component")
      .addFunction("parent", &get_parent<Afk::PhysicsBody>)
      .addFunction("apply_force", &Afk::PhysicsBody::apply_force)
      .addFunction("apply_torque", &Afk::PhysicsBody::apply_torque)
      .addFunction("translate", &Afk::PhysicsBody::translate)
      .endClass()

      .beginClass<Afk::Transform>("transform_component")
      .addFunction("parent", &get_parent<Afk::Transform>)
      .addData("translation", &Afk::Transform::translation)
      .addData("rotation", &Afk::Transform::rotation)
      .addData("scale", &Afk::Transform::scale)
      .endClass()

      .beginClass<Afk::ModelSource>("model_component")
      .addFunction("parent", &get_parent<Afk::ModelSource>)
      .addData("name", &Afk::ModelSource::name)
      .endClass()

      .beginClass<Afk::BaseComponent>("component")
      .addFunction("parent", &get_parent<Afk::BaseComponent>)
      .endClass()

      .beginNamespace("engine")
      .addFunction("delta_time", &get_delta_time)
      .endNamespace();
}
