#include "afk/Afk.hpp"

#include <memory>
#include <string>
#include <utility>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "afk/asset/AssetFactory.hpp"
#include "afk/component/LuaScript.hpp"
#include "afk/component/ScriptsComponent.hpp"
#include "afk/debug/Assert.hpp"
#include "afk/ecs/GameObject.hpp"
#include "afk/io/Log.hpp"
#include "afk/io/ModelSource.hpp"
#include "afk/physics/PhysicsBody.hpp"
#include "afk/physics/RigidBodyType.hpp"
#include "afk/physics/shape/Box.hpp"
#include "afk/physics/shape/Sphere.hpp"
#include "afk/renderer/ModelRenderSystem.hpp"
#include "afk/script/Bindings.hpp"
#include "afk/script/LuaInclude.hpp"

using namespace std::string_literals;

using glm::vec3;
using glm::vec4;

using Afk::Engine;
using Afk::Event;
using Afk::Texture;
using Action   = Afk::Event::Action;
using Movement = Afk::Camera::Movement;

auto Engine::initialize() -> void {
  afk_assert(!this->is_initialized, "Engine already initialized");

  this->renderer.initialize();
  this->event_manager.initialize(this->renderer.window);
  this->ui.initialize(this->renderer.window);
  this->terrain_manager.initialize();
  this->lua = luaL_newstate();
  luaL_openlibs(this->lua);
  Afk::LuaScript::setup_lua_state(this->lua);
  Afk::add_engine_bindings(this->lua);
  this->terrain_manager.generate_terrain(500, 500, 0.05f, 7.5f);

  this->renderer.load_model(this->terrain_manager.get_model());

  const auto city_entity     = registry.create();
  auto city_transform        = Transform{city_entity};
  city_transform.scale       = vec3{0.25f};
  city_transform.translation = vec3{0.0f, -1.0f, 0.0f};
  registry.assign<Afk::Transform>(city_entity, city_transform);
  registry.assign<Afk::ModelSource>(city_entity, city_entity, "res/model/city/city.fbx");
  registry.assign<Afk::PhysicsBody>(city_entity, city_entity, &this->physics_body_system,
                                    city_transform, 0.0f, 0.0f, 0.0f, 0.0f,
                                    false, Afk::RigidBodyType::STATIC,
                                    Afk::Box{100000000.0f, 0.1f, 100000000.0f});

  Afk::Asset::game_asset_factory("asset/basketball.lua");

  auto cam = registry.create();
  registry.assign<Afk::ScriptsComponent>(cam, cam)
      .add_script("script/camera_keyboard_control.lua", this->lua, &this->event_manager)
      .add_script("script/camera_mouse_control.lua", this->lua, &this->event_manager);
  this->is_initialized = true;
}

auto Engine::get() -> Engine & {
  static auto instance = Engine{};

  return instance;
}

auto Engine::exit() -> void {
  this->is_running = false;
}

auto Engine::render() -> void {
  auto terrain_transform        = Transform{};
  terrain_transform.translation = vec3{-250.0f, -20.0f, -250.0f};
  this->renderer.queue_draw({"gen/terrain/terrain", "shader/terrain.prog", terrain_transform});
  Afk::queue_models(&this->registry, &this->renderer, "shader/default.prog");

  this->renderer.clear_screen({135.0f, 206.0f, 235.0f, 1.0f});
  this->ui.prepare();
  this->renderer.draw();
  this->ui.draw();
  this->renderer.swap_buffers();
}

auto Engine::update() -> void {
  this->event_manager.pump_events();

  if (glfwWindowShouldClose(this->renderer.window)) {
    this->is_running = false;
  }

  if (this->ui.show_menu) {
    glfwSetInputMode(this->renderer.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else {
    glfwSetInputMode(this->renderer.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  // this->update_camera();

  this->physics_body_system.update(&this->registry, this->get_delta_time());

  ++this->frame_count;
  this->last_update = Afk::Engine::get_time();
}

auto Engine::get_time() -> float {
  return static_cast<float>(glfwGetTime());
}

auto Engine::get_delta_time() -> float {
  return this->get_time() - this->last_update;
}

auto Engine::get_is_running() const -> bool {
  return this->is_running;
}
