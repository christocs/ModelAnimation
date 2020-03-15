#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Mesh.hpp"

class ModelLoader;
class Shader;

class Model {
  private:
    friend ModelLoader;

    std::string dir               = "";
    std::string path              = "";
    std::vector<Mesh> meshes      = {};
    std::vector<Texture> textures = {};
    Transform transform           = {};

  public:
    Model() = default;
    Model(std::string const &_path);

    auto draw(Shader shader) const -> void;

    auto getRotation() const -> glm::quat;
    auto setRotation(glm::quat rotation) -> void;

    auto getScale() const -> glm::vec3;
    auto setScale(glm::vec3 scale) -> void;

    auto getTranslation() const -> glm::vec3;
    auto setTranslation(glm::vec3 translation) -> void;
};
