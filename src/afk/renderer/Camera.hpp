#pragma once

#include <glm/glm.hpp>

namespace Afk {
  class Camera {
  public:
    static constexpr glm::vec3 WORLD_UP = {0.0f, 1.0f, 0.0f};

    enum class Movement { Forward, Backward, Left, Right };

    auto handle_mouse(float deltaX, float deltaY) -> void;
    auto handle_key(Movement movement, float deltaTime) -> void;

    auto get_view_matrix() -> glm::mat4;
    auto get_projection_matrix(int width, int height) const -> glm::mat4;
    auto get_position() const -> glm::vec3;
    auto get_angles() const -> glm::vec2;

  private:
    float fov         = 75.0f;
    float near        = 0.1f;
    float far         = 1000.0f;
    float speed       = 10.0f;
    float sensitivity = 0.1f;

    auto get_front() const -> glm::vec3;
    auto get_right() const -> glm::vec3;
    auto get_up() const -> glm::vec3;

    glm::vec2 angles   = {};
    glm::vec3 position = {};
  };
}
