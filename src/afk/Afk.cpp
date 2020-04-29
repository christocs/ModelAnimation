#include "afk/Afk.hpp"

#include <memory>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "afk/debug/Assert.hpp"
#include "afk/io/Log.hpp"

using namespace std::string_literals;

using glm::vec3;

using Afk::Engine;
using Afk::Event;
using Action   = Afk::Event::Action;
using Movement = Afk::Camera::Movement;

auto Engine::move_mouse(Event event) -> void {
  const auto data = std::get<Event::MouseMove>(event.data);

  static auto last_x      = 0.0f;
  static auto last_y      = 0.0f;
  static auto first_frame = true;

  const auto dx = static_cast<float>(data.x) - last_x;
  const auto dy = static_cast<float>(data.y) - last_y;

  if (!first_frame && !this->ui.is_visible) {
    this->camera.handle_mouse(dx, dy);
  } else {
    first_frame = false;
  }

  last_x = static_cast<float>(data.x);
  last_y = static_cast<float>(data.y);
}

// FIXME: Move to key handler.
auto Engine::move_keyboard(Event event) -> void {
  const auto key = std::get<Event::Key>(event.data).key;

  if (event.type == Event::Type::KeyDown && key == GLFW_KEY_ESCAPE) {
    this->is_running = false;
  } else if (event.type == Event::Type::KeyDown && key == GLFW_KEY_N) {
    this->ui.is_visible = !this->ui.is_visible;
  } else if (event.type == Event::Type::KeyDown && key == GLFW_KEY_M) {
    this->renderer.wireframe_enabled = !this->renderer.wireframe_enabled;
  }
}

// FIXME: Move someone more appropriate.
static auto resize_window_callback([[maybe_unused]] GLFWwindow *window,
                                   int width, int height) -> void {
  auto &afk = Engine::get();
  afk.renderer.set_viewport(0, 0, width, height);
}

auto Engine::get() -> Engine & {
  static auto instance = Engine{};

  return instance;
}

// FIXME: Move somewhere more appropriate.
auto Engine::update_camera() -> void {
  if (!this->ui.is_visible) {
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

Engine::Engine() {
  this->event_manager.setup_callbacks(this->renderer.window);

  this->event_manager.register_event(Event::Type::MouseMove,
                                     Afk::EventManager::Callback{[this](Event event) {
                                       this->move_mouse(event);
                                     }});
  this->event_manager.register_event(Event::Type::KeyDown,
                                     Afk::EventManager::Callback{[this](Event event) {
                                       this->move_keyboard(event);
                                     }});

  glfwSetFramebufferSizeCallback(this->renderer.window, resize_window_callback);
}

auto Engine::render() -> void {
  const auto &shader = this->renderer.get_shader_program("shader/default.prog");
  const auto &model  = this->renderer.get_model("res/model/city/city.fbx");
  const auto window_size = this->renderer.get_window_size();

  this->renderer.clear_screen();
  this->ui.prepare();

  // FIXME: Move to a scene manager.
  this->renderer.use_shader(shader);
  this->renderer.set_uniform(
      shader, "u_matrices.projection",
      this->camera.get_projection_matrix(window_size.x, window_size.y));
  this->renderer.set_uniform(shader, "u_matrices.view", this->camera.get_view_matrix());

  auto transform        = Transform{};
  transform.translation = vec3{0.0f, -1.0f, 0.0f};
  this->renderer.draw_model(model, shader, transform);
  this->ui.draw();
  this->renderer.swap_buffers();
}

auto Engine::update() -> void {
  this->event_manager.pump_events();

  if (glfwWindowShouldClose(this->renderer.window)) {
    this->is_running = false;
  }

  if (this->ui.is_visible) {
    glfwSetInputMode(this->renderer.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else {
    glfwSetInputMode(this->renderer.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  this->update_camera();

  ++this->frame_count;
  this->last_update = this->get_time();
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
