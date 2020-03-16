#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "afk/render/Mesh.hpp"

namespace Afk {
    namespace Io {
        class ModelLoader;
    };

    namespace Render {
        class Shader;

        class Model {
          public:
            using Meshes   = std::vector<Mesh>;
            using Textures = std::vector<Texture>;

          private:
            friend Io::ModelLoader;

            std::string dir     = "";
            std::string path    = "";
            Meshes meshes       = {};
            Textures textures   = {};
            Transform transform = {};

          public:
            Model() = default;
            Model(std::string const &_path);
            Model(Model &&)      = default;
            Model(const Model &) = default;
            auto operator=(Model &&) -> Model & = default;
            auto operator=(const Model &) -> Model & = default;

            auto draw(Shader &shader) const -> void;

            auto getRotation() const -> glm::quat;
            auto setRotation(glm::quat rotation) -> void;

            auto getScale() const -> glm::vec3;
            auto setScale(glm::vec3 scale) -> void;

            auto getTranslation() const -> glm::vec3;
            auto setTranslation(glm::vec3 translation) -> void;
        };
    };
};

