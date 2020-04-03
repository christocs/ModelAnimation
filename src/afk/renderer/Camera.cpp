#include "afk/renderer/Camera.hpp"

#include <algorithm>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using glm::mat4;
using glm::vec3;

using Afk::Camera;

auto Camera::handleMouse(float deltaX, float deltaY) -> void {
  constexpr auto maxYaw = 89.0f;

  this->angles.x += deltaX * this->sensitivity;
  this->angles.y += -deltaY * this->sensitivity;
  this->angles.y = std::clamp(this->angles.y, -maxYaw, maxYaw);
}

auto Camera::handleKey(Movement movement, float deltaTime) -> void {
  const auto velocity = this->speed * deltaTime;

  switch (movement) {
    case Movement::Forward: {
      this->position += this->getFront() * velocity;
    } break;
    case Movement::Backward: {
      this->position -= this->getFront() * velocity;
    } break;
    case Movement::Left: {
      this->position -= this->getRight() * velocity;
    } break;
    case Movement::Right: {
      this->position += this->getRight() * velocity;
    } break;
  }
}

auto Camera::getViewMatrix() -> mat4 {
  return glm::lookAt(this->position, this->position + this->getFront(), this->getUp());
}

auto Camera::getProjectionMatrix(unsigned width, unsigned height) const -> mat4 {
  const auto w = static_cast<float>(width);
  const auto h = static_cast<float>(height);

  const auto projectionMatrix =
      glm::perspective(glm::radians(this->fov), w / h, this->near, this->far);

  return projectionMatrix;
}

auto Camera::getFront() const -> vec3 {
  auto front = vec3{};

  front.x = std::cos(glm::radians(this->angles.x)) *
            std::cos(glm::radians(this->angles.y));
  front.y = std::sin(glm::radians(this->angles.y));
  front.z = std::sin(glm::radians(this->angles.x)) *
            std::cos(glm::radians(this->angles.y));

  front = glm::normalize(front);

  return front;
}

auto Camera::getRight() const -> vec3 {
  return glm::normalize(glm::cross(this->getFront(), this->WORLD_UP));
}

auto Camera::getUp() const -> vec3 {
  return glm::normalize(glm::cross(this->getRight(), this->getFront()));
}
