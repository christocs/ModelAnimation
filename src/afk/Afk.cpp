#include "afk/Afk.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "afk/io/Log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using Afk::Engine;
using glm::vec3;

auto Engine::move_mouse(const Event &evt) -> void {
  auto move = std::get<Event::MouseMove>(evt.data);

  static auto last_x      = 0.0f;
  static auto last_y      = 0.0f;
  static auto first_frame = true;

  const auto dx = static_cast<float>(move.mouse_x) - last_x;
  const auto dy = static_cast<float>(move.mouse_y) - last_y;

  if (!first_frame) {
    this->camera.handle_mouse(dx, dy);
  } else {
    first_frame = false;
  }

  last_x = static_cast<float>(move.mouse_x);
  last_y = static_cast<float>(move.mouse_y);
}

static auto resize_window_callback([[maybe_unused]] GLFWwindow *window,
                                   int width, int height) -> void {
  auto &afk = Engine::get();

  afk.renderer.set_viewport(0, 0, static_cast<unsigned>(width),
                            static_cast<unsigned>(height));
}

auto Engine::get() -> Engine & {
  static auto instance = Engine{};

  return instance;
}

Engine::Engine() {
  this->events.setup_callbacks(this->renderer.window.get());
  this->events.register_event(
      Afk::Event::EventType::MouseMove,
      std::bind(&Afk::Engine::move_mouse, this, std::placeholders::_1));
  this->events.register_event(
      Afk::Event::EventType::KeyDown,
      std::bind(&Afk::Engine::move_keyboard, this, std::placeholders::_1));
  glfwSetFramebufferSizeCallback(this->renderer.window.get(), resize_window_callback);
  glfwSetInputMode(this->renderer.window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

auto Engine::move_keyboard(const Event &e) -> void {
  auto evt = std::get<Afk::Event::Key>(e.data);
  if (evt.key_code == GLFW_KEY_W) {
    this->camera.handle_key(Camera::Movement::Forward, this->get_delta_time());
  }

  if (evt.key_code == GLFW_KEY_A) {
    this->camera.handle_key(Camera::Movement::Left, this->get_delta_time());
  }

  if (evt.key_code == GLFW_KEY_S) {
    this->camera.handle_key(Camera::Movement::Backward, this->get_delta_time());
  }

  if (evt.key_code == GLFW_KEY_D) {
  }

  if (glfwGetKey(this->renderer.window.get(), GLFW_KEY_ESCAPE)) {
    this->is_running = false;
  }
}

auto Engine::render() -> void {
  auto shader = this->renderer.get_shader_program("default");
  auto model  = this->renderer.get_model("res/model/city/city.fbx");
  const auto [width, height] = this->renderer.get_window_size();

  this->renderer.clear_screen();

  // FIXME
  this->renderer.use_shader(shader);
  this->renderer.set_uniform(shader, "u_matrices.projection",
                             this->camera.get_projection_matrix(width, height));
  this->renderer.set_uniform(shader, "u_matrices.view", this->camera.get_view_matrix());

  auto transform        = Transform{};
  transform.translation = vec3{0.0f, -1.0f, 0.0f};

  this->renderer.draw_model(model, shader, transform);
  this->renderer.swap_buffers();
}

auto Engine::update() -> void {
  this->events.pump_events();

  if (glfwWindowShouldClose(this->renderer.window.get())) {
    this->is_running = false;
  }

  if ((this->get_time() - this->last_fps_update) >= 1.0f) {
    Afk::status << "FPS: " << this->fps_count << '\n';
    this->last_fps_update = this->get_time();
    this->fps_count       = 0;
  }

  ++this->fps_count;
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
