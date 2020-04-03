#pragma once

#include <glm/glm.hpp>

namespace Afk {
  class Camera {
  public:
    static constexpr glm::vec3 WORLD_UP = {0.0f, 1.0f, 0.0f};

    enum class Movement { Forward, Backward, Left, Right };

    auto handleMouse(float deltaX, float deltaY) -> void;
    auto handleKey(Movement movement, float deltaTime) -> void;

    auto getViewMatrix() -> glm::mat4;
    auto getProjectionMatrix(unsigned width, unsigned height) const -> glm::mat4;

  private:
    float fov         = 75.0f;
    float near        = 25.0;
    float far         = 10000.0f;
    float speed       = 500.0f;
    float sensitivity = 0.05f;

    auto getFront() const -> glm::vec3;
    auto getRight() const -> glm::vec3;
    auto getUp() const -> glm::vec3;

    glm::vec2 angles   = {};
    glm::vec3 position = {};
  };
}
