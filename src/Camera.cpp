#include "Camera.hpp"

#include <algorithm>
#include <cmath>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

Camera::Camera(vec3 _position, float _yaw, float _pitch)
    : position(_position), yaw(_yaw), pitch(_pitch) {

    this->updateCameraVectors();
}

auto Camera::getFov() const -> float {
    return this->fov;
}

auto Camera::getViewMatrix() const -> glm::mat4 {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

auto Camera::processKeys(Movement direction, float deltaTime) -> void {
    const auto velocity = this->speed * deltaTime;

    switch (direction) {
        case FORWARD: this->position += this->front * velocity; break;
        case BACKWARD: this->position -= this->front * velocity; break;
        case LEFT: this->position -= this->right * velocity; break;
        case RIGHT: this->position += this->right * velocity; break;
    }
}

auto Camera::processMouse(float xoffset, float yoffset) -> void {
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;
    this->pitch = std::clamp(this->pitch, -89.0f, 89.0f);

    this->updateCameraVectors();
}

auto Camera::updateCameraVectors() -> void {
    this->front.x =
        std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
    this->front.y = std::sin(glm::radians(this->pitch));
    this->front.z =
        std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));

    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up    = glm::normalize(glm::cross(this->right, this->front));
}
