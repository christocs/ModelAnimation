#pragma once

#include <vector>

#include <glm/glm.hpp>

enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
  public:
    static constexpr auto YAW         = -90.0f;
    static constexpr auto PITCH       = 0.0f;
    static constexpr auto SPEED       = 2.5f;
    static constexpr auto SENSITIVITY = 0.1f;
    static constexpr auto FOV         = 45.0f;

    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right;
    glm::vec3 worldUp = up;

    float yaw         = YAW;
    float pitch       = PITCH;
    float speed       = SPEED;
    float sensitivity = SENSITIVITY;
    float fov         = FOV;

  public:
    Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), float _yaw = YAW,
           float _pitch = PITCH);

    auto getFov() const -> float;
    auto getViewMatrix() const -> glm::mat4;
    auto processKeys(Movement direction, float deltaTime) -> void;
    auto processMouse(float xoffset, float yoffset) -> void;

  private:
    auto updateCameraVectors() -> void;
};
