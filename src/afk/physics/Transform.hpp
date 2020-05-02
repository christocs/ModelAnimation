#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "afk/component/BaseComponent.hpp"

namespace Afk {
  /**
   * transform component
   */
  struct Transform : public BaseComponent {
    /**
     * translation
     */
    glm::vec3 translation = glm::vec3{1.0f};
    /**
     * scale
     */
    glm::vec3 scale = glm::vec3{1.0f};
    /**
     * rotation
     */
    glm::quat rotation = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};
    /**
     * constructor
     */
    Transform() = default;
    /**
     * construct owned component
     * @param e owner
     */
    Transform(GameObject e);
    /**
     * construct with transformation
     * @param transform transform
     */
    Transform(glm::mat4 transform);
    /**
     * construct owned component with transformation
     * @param e owner
     * @param transform transform
     */
    Transform(GameObject e, glm::mat4 transform);
  };
}
