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
#include "afk/debug/Assert.hpp"
#include "afk/ecs/GameObject.hpp"
#include "afk/io/Log.hpp"
#include "afk/io/ModelSource.hpp"
#include "afk/physics/PhysicsBody.hpp"
#include "afk/physics/RigidBodyType.hpp"
#include "afk/physics/shape/Box.hpp"
#include "afk/physics/shape/Sphere.hpp"
#include "afk/renderer/ModelRenderSystem.hpp"
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
  const int terrain_width  = 64;
  const int terrain_length = 64;
  this->terrain_manager.generate_terrain(terrain_width, terrain_length, 0.05f, 7.5f);
  this->lua = luaL_newstate();
  luaL_openlibs(this->lua);
  Afk::LuaScript::setup_lua_state(this->lua);
  this->terrain_manager.generate_terrain(500, 500, 0.05f, 7.5f);
  
  this->renderer.load_model(this->terrain_manager.get_model());

  // FIXME: Move to key manager
  this->event_manager.register_event(Event::Type::MouseMove,
                                     Afk::EventManager::Callback{[this](Event event) {
                                       this->move_mouse(event);
                                     }});
  this->event_manager.register_event(Event::Type::KeyDown,
                                     Afk::EventManager::Callback{[this](Event event) {
                                       this->move_keyboard(event);
                                     }});

  auto terrain_entity           = registry.create();
  auto terrain_transform        = Transform{terrain_entity};
  terrain_transform.translation = glm::vec3{0, 0, 0};
  registry.assign<Afk::ModelSource>(terrain_entity, terrain_entity, "gen/terrain");
  registry.assign<Afk::Transform>(terrain_entity, terrain_entity);
  //  const auto a = this->terrain_manager.get_height_map(terrain_width, terrain_length);
  registry.assign<Afk::PhysicsBody>(terrain_entity, terrain_entity, &this->physics_body_system,
                                    terrain_transform, 0.3f, 0.0f, 0.0f, 0.0f,
                                    true, Afk::RigidBodyType::STATIC,
                                    this->terrain_manager.height_map);

  Afk::Asset::game_asset_factory("asset/basketball.lua");

  this->is_initialized = true;
}

auto Engine::move_mouse(Event event) -> void {
  const auto data = std::get<Event::MouseMove>(event.data);

  static auto last_x      = 0.0f;
  static auto last_y      = 0.0f;
  static auto first_frame = true;

  const auto dx = static_cast<float>(data.x) - last_x;
  const auto dy = static_cast<float>(data.y) - last_y;

  if (!first_frame && !this->ui.show_menu) {
    this->camera.handle_mouse(dx, dy);
  } else {
    first_frame = false;
  }

  last_x = static_cast<float>(data.x);
  last_y = static_cast<float>(data.y);
}

auto Engine::get() -> Engine & {
  static auto instance = Engine{};

  return instance;
}

auto Engine::exit() -> void {
  this->is_running = false;
}

// FIXME: Move to key handler.
// TODO: Assign these keys in Lua
auto Engine::move_keyboard(Event event) -> void {
  const auto key = std::get<Event::Key>(event.data).key;

  if (event.type == Event::Type::KeyDown && key == GLFW_KEY_ESCAPE) {
    this->exit();
  } else if (event.type == Event::Type::KeyDown && key == GLFW_KEY_GRAVE_ACCENT) {
    this->ui.show_menu = !this->ui.show_menu;
  } else if (event.type == Event::Type::KeyDown && key == GLFW_KEY_1) {
    this->renderer.set_wireframe(!this->renderer.get_wireframe());
  }
}

// FIXME: Move somewhere more appropriate.
auto Engine::update_camera() -> void {
  if (!this->ui.show_menu) {
    if (this->event_manager.key_state.at(Action::Forward)) {
      this->camera.handle_key(Movement::Forward, this->get_delta_time());
    }

    if (this->event_manager.key_state.at(Action::Left)) {
      this->camera.handle_key(Movement::Left, this->get_delta_time());
    }

    if (this->event_manager.key_state.at(Action::Backward)) {
      this->camera.handle_key(Movement::Backward, this->get_delta_time());
    }

    if (this->event_manager.key_state.at(Action::Right)) {
      this->camera.handle_key(Movement::Right, this->get_delta_time());
    }
  }
}

auto Engine::render() -> void {
  // FIXME: Support multiple shader programs properly
  const auto &shader = this->renderer.get_shader_program("shader/default.prog");
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

  this->update_camera();

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
