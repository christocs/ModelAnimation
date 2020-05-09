#pragma once

#include <unordered_map>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Afk {
  struct Animation {

    struct AnimationNode {
      struct PositionKey {
        glm::vec3 position = {};
        // time of key
        double time;
      };
      struct ScaleKey {
        glm::vec3 scale = {};
        // time of key
        double time;
      };
      struct RotationKey {
        glm::quat rotation = {};
        // time of key
        double time;
      };

      std::vector<PositionKey> position_keys;
      std::vector<ScaleKey> scaling_keys;
      std::vector<RotationKey> rotation_keys;
    };

    using AnimationNodes = std::unordered_map<unsigned int, AnimationNode>;

    AnimationNodes animation_nodes = {};
    // duration of animation in ticks
    double duration = 0;
    // ticks per second
    double ticks_per_second = 0;
  };
}
