#pragma once

#include <map>
#include <unordered_map>
#include <vector>

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

      using PositionKeys = std::vector<PositionKey>;
      using ScaleKeys    = std::vector<ScaleKey>;
      using RotationKeys = std::vector<RotationKey>;

      PositionKeys position_keys;
      ScaleKeys scaling_keys;
      RotationKeys rotation_keys;
    };

    using AnimationNodes = std::unordered_map<unsigned int, AnimationNode>;

    AnimationNodes animation_nodes = {};
    // duration of animation in ticks
    double duration = 0;
    // ticks per second
    double ticks_per_second = 0;
  };
}
