#include "afk/render/Model.hpp"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "afk/io/ModelLoader.hpp"
#include "afk/render/Shader.hpp"

using glm::mat4;
using glm::quat;
using glm::vec3;
using std::string;

using Afk::Io::ModelLoader;
using Afk::Render::Model;
using Afk::Render::Shader;

Model::Model(const string &_path) {
    auto model = ModelLoader{}.load(_path);

    this->dir      = std::move(model.dir);
    this->path     = std::move(model.path);
    this->meshes   = std::move(model.meshes);
    this->textures = std::move(model.textures);
}

auto Model::draw(Shader &shader) const -> void {
    for (const auto &mesh : this->meshes) {
        mesh.draw(shader, this->transform);
    }
}

auto Model::getRotation() const -> glm::quat {
    return this->transform.rotation;
}

auto Model::setRotation(quat rotation) -> void {
    this->transform.rotation = rotation;
}

auto Model::getScale() const -> glm::vec3 {
    return this->transform.scale;
}

auto Model::setScale(vec3 scale) -> void {
    this->transform.scale = scale;
}

auto Model::getTranslation() const -> glm::vec3 {
    return this->transform.translation;
}

auto Model::setTranslation(vec3 translation) -> void {
    this->transform.translation = translation;
}
