#pragma once

#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Afk {
  struct Bone {
    glm::mat4 offset_transform = glm::mat4(1.0f);
    glm::mat4 final_transform  = glm::mat4(1.0f);
  };

  using Bones         = std::vector<Bone>;
  using BoneNodeMap   = std::unordered_map<unsigned int, unsigned int>;
  using BoneStringMap = std::unordered_map<std::string, unsigned int>;
}
