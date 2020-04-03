#include "afk/Afk.hpp"

#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "afk/io/Log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using Afk::Engine;
using Afk::Log;
using glm::vec3;

// FIXME: Move to event manager
static auto mouseCallback([[maybe_unused]] GLFWwindow *window, double x, double y) -> void {
  static auto lastX      = 0.0f;
  static auto lastY      = 0.0f;
  static auto firstFrame = true;

  auto &afk = Engine::get();

  const auto dx = static_cast<float>(x) - lastX;
  const auto dy = static_cast<float>(y) - lastY;

  if (!firstFrame) {
    afk.camera.handleMouse(dx, dy);
  } else {
    firstFrame = false;
  }

  lastX = static_cast<float>(x);
  lastY = static_cast<float>(y);
}

auto Engine::get() -> Engine & {
  static auto instance = Engine{};

  return instance;
}

Engine::Engine() {
  glfwSetCursorPosCallback(this->renderer.window.get(), mouseCallback);
  glfwSetInputMode(this->renderer.window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// FIXME: Move to event manager
auto Engine::handleKeys() -> void {

  if (glfwGetKey(this->renderer.window.get(), GLFW_KEY_W)) {
    this->camera.handleKey(Camera::Movement::Forward, this->getDeltaTime());
  }

  if (glfwGetKey(this->renderer.window.get(), GLFW_KEY_A)) {
    this->camera.handleKey(Camera::Movement::Left, this->getDeltaTime());
  }

  if (glfwGetKey(this->renderer.window.get(), GLFW_KEY_S)) {
    this->camera.handleKey(Camera::Movement::Backward, this->getDeltaTime());
  }

  if (glfwGetKey(this->renderer.window.get(), GLFW_KEY_D)) {
    this->camera.handleKey(Camera::Movement::Right, this->getDeltaTime());
  }

  if (glfwGetKey(this->renderer.window.get(), GLFW_KEY_ESCAPE)) {
    this->isRunning = false;
  }
}

auto Engine::render() -> void {
  auto shader = this->renderer.getShaderProgram("default");
  auto model  = this->renderer.getModel("res/model/city/city.fbx");
  const auto [width, height] = this->renderer.getWindowSize();

  this->renderer.clearScreen();

  // FIXME
  this->renderer.useShader(shader);
  this->renderer.setUniform(shader, "projection",
                            this->camera.getProjectionMatrix(width, height));
  this->renderer.setUniform(shader, "view", this->camera.getViewMatrix());

  auto transform        = Transform{};
  transform.scale       = vec3{0.25f};
  transform.translation = vec3{0.0f, -50.0f, 0.0f};
  transform.rotation = glm::angleAxis(glm::radians(-90.0f), vec3{1.0f, 0.0f, 0.0f});

  this->renderer.drawModel(model, shader, transform);
  this->renderer.swapBuffers();
}

auto Engine::update() -> void {
  glfwPollEvents();

  if (glfwWindowShouldClose(this->renderer.window.get())) {
    this->isRunning = false;
  }

  this->handleKeys();

  if ((this->getTime() - this->lastFpsUpdate) >= 1.0f) {
    Log::status("FPS: " + std::to_string(this->fpsCount));
    this->lastFpsUpdate = this->getTime();
    this->fpsCount      = 0;
  }

  ++this->fpsCount;
  this->lastUpdate = this->getTime();
}

auto Engine::getTime() -> float {
  return static_cast<float>(glfwGetTime());
}

auto Engine::getDeltaTime() -> float {
  return this->getTime() - this->lastUpdate;
}

auto Engine::getIsRunning() const -> bool {
  return this->isRunning;
}
