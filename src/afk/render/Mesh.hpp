#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Afk {
    namespace Render {
        class Shader;

        struct Vertex {
            glm::vec3 position  = {};
            glm::vec3 normal    = {};
            glm::vec2 uvs       = {};
            glm::vec3 tangent   = {};
            glm::vec3 bitangent = {};
        };

        struct Texture {
            GLuint id        = {};
            std::string type = {};
            std::string path = {};
        };

        struct Transform {
            glm::vec3 translation = glm::vec3{1.0f};
            glm::vec3 scale       = glm::vec3{1.0f};
            glm::quat rotation    = glm::quat{1.0f, 0.0f, 0.0f, 0.0f};

            Transform() = default;
            Transform(glm::mat4 transform);
            Transform(Transform &&)      = default;
            Transform(const Transform &) = default;
            auto operator=(const Transform &) -> Transform & = default;
            auto operator=(Transform &&) -> Transform & = default;
        };

        class Mesh {
          public:
            using Index    = unsigned;
            using Vertices = std::vector<Vertex>;
            using Indices  = std::vector<Index>;
            using Textures = std::vector<Texture>;

            Mesh(Vertices &&_vertices, Indices &&_indices, Textures &&_textures,
                 glm::mat4 &&_transform);

            // FIXME: Decouple from OpenGL.
            Mesh()             = default;
            Mesh(const Mesh &) = default;
            Mesh(Mesh &&)      = default;
            auto operator=(const Mesh &) -> Mesh & = default;
            auto operator=(Mesh &&) -> Mesh & = default;

            auto draw(const Shader &shader, Transform parentTransform) const -> void;

          private:
            GLuint vao = {};
            GLuint vbo = {};
            GLuint ibo = {};

            Vertices vertices   = {};
            Indices indices     = {};
            Textures textures   = {};
            Transform transform = {};

            auto setupMesh() -> void;
        };
    };
};
