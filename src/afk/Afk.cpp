#include "afk/Afk.hpp"

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "afk/io/Log.hpp"

using Afk::Engine;
using Afk::Log;
using glm::vec3;

// FIXME: Tidy
auto Engine::handleMouse() -> void {
  static auto firstFrame = true;

  const auto [windowW, windowH] =
      static_cast<sf::Vector2i>(this->renderer.window.getSize());
  const auto centerX = windowW / 2;
  const auto centerY = windowH / 2;

  const auto [x, y] =
      static_cast<sf::Vector2i>(sf::Mouse::getPosition(this->renderer.window));
  const auto offsetX = x - centerX;
  const auto offsetY = centerY - y;

  if (!firstFrame) {
    this->camera.handleMouse(offsetX, offsetY);
  } else {
    firstFrame = false;
  }

  sf::Mouse::setPosition(sf::Vector2i{centerX, centerY}, this->renderer.window);
}

// FIXME: Tidy
auto Engine::handleKeys() -> void {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    this->camera.handleKey(Camera::Movement::Forward, this->getDeltaTime());
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    this->camera.handleKey(Camera::Movement::Left, this->getDeltaTime());
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    this->camera.handleKey(Camera::Movement::Backward, this->getDeltaTime());
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    this->camera.handleKey(Camera::Movement::Right, this->getDeltaTime());
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
    this->isRunning = false;
  }
}

auto Engine::render() -> void {
  auto shader = this->renderer.getShaderProgram("default");
  auto model  = this->renderer.getModel("res/model/city/city.fbx");
  const auto [width, height] = this->renderer.window.getSize();

  this->renderer.clearScreen();

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
  auto event = sf::Event{};
  while (this->renderer.window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      this->isRunning = false;
    } else if (event.type == sf::Event::Resized) {
      this->renderer.setViewport(0, 0, event.size.width, event.size.height);
    } else if (event.type == sf::Event::GainedFocus) {
      this->renderer.window.setMouseCursorVisible(false);
      this->hasFocus = true;
    } else if (event.type == sf::Event::LostFocus) {
      this->hasFocus = false;
    }
  }

  if (hasFocus) {
    this->handleMouse();
    this->handleKeys();
  }

  if ((this->getTime() - this->lastFpsUpdate) >= 1.0f) {
    Log::status("FPS: " + std::to_string(this->fpsCount));
    this->lastFpsUpdate = this->getTime();
    this->fpsCount      = 0;
  }

  ++this->fpsCount;
  this->lastUpdate = this->getTime();
}

auto Engine::getTime() -> float {
  return this->clock.getElapsedTime().asSeconds();
}

auto Engine::getDeltaTime() -> float {
  return this->getTime() - this->lastUpdate;
}

auto Engine::getIsRunning() const -> bool {
  return this->isRunning;
}
