#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Afk {
    namespace Render {
        class Camera {
          public:
            enum class Movement { FORWARD, BACKWARD, LEFT, RIGHT };

            static constexpr auto DEFAULT_YAW         = -90.0f;
            static constexpr auto DEFAULT_PITCH       = 0.0f;
            static constexpr auto DEFAULT_SPEED       = 1000.0f;
            static constexpr auto DEFAULT_SENSITIVITY = 0.1f;
            static constexpr auto DEFAULT_FOV         = 45.0f;

            glm::vec3 position;
            glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 right;
            glm::vec3 worldUp = up;

            float yaw         = DEFAULT_YAW;
            float pitch       = DEFAULT_PITCH;
            float speed       = DEFAULT_SPEED;
            float sensitivity = DEFAULT_SENSITIVITY;
            float fov         = DEFAULT_FOV;

          public:
            Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
                   float _yaw = DEFAULT_YAW, float _pitch = DEFAULT_PITCH);
            Camera(const Camera &) = delete;
            Camera(Camera &&)      = default;
            auto operator=(const Camera &) -> Camera & = delete;
            auto operator=(Camera &&) -> Camera & = default;

            auto getFov() const -> float;
            auto getViewMatrix() const -> glm::mat4;
            auto processKeys(Movement direction, float deltaTime) -> void;
            auto processMouse(float xoffset, float yoffset) -> void;

          private:
            auto updateCameraVectors() -> void;
        };
    };
};
