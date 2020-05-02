#pragma once

#include <glm/glm.hpp>

namespace Afk {
  class Camera {
  public:
    static constexpr glm::vec3 WORLD_UP = {0.0f, 1.0f, 0.0f};

    enum class Movement { Forward, Backward, Left, Right };

    /**
     * unused
     * @todo remove
     */
    auto handle_mouse(float deltaX, float deltaY) -> void;
    /**
     * unused
     * @todo remove
     */
    auto handle_key(Movement movement, float deltaTime) -> void;

    auto get_view_matrix() const -> glm::mat4;
    auto get_projection_matrix(int width, int height) const -> glm::mat4;
    auto get_position() const -> glm::vec3;
    auto get_angles() const -> glm::vec2;
    auto set_position(glm::vec3 v) -> void;
    auto set_angles(glm::vec2 v) -> void;
    auto get_front() const -> glm::vec3;
    auto get_right() const -> glm::vec3;
    auto get_up() const -> glm::vec3;

  private:
    float fov         = 75.0f;
    float near        = 0.1f;
    float far         = 1000.0f;
    float speed       = 10.0f;
    float sensitivity = 0.1f;

    glm::vec2 angles   = {};
    glm::vec3 position = {};
  };
}
