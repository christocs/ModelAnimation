#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Afk {
    namespace Render {
        class Shader {
          private:
            GLuint id = {};

          public:
            Shader() = default;
            Shader(const std::string &vertexPath, const std::string &fragmentPath);
            Shader(Shader &&)      = default;
            Shader(const Shader &) = delete;
            auto operator=(const Shader &) -> Shader & = delete;
            auto operator=(Shader &&) -> Shader & = default;

            auto getId() const -> GLuint;

            auto use() const -> void;
            auto compileShader(const std::string &path, GLuint type) -> GLuint;
            auto linkShader(GLuint vertexShader, GLuint fragmentShader) -> GLuint;

            auto setUniform(const std::string &name, bool value) const -> void;
            auto setUniform(const std::string &name, int value) const -> void;
            auto setUniform(const std::string &name, unsigned value) const -> void;
            auto setUniform(const std::string &name, float value) const -> void;
            auto setUniform(const std::string &name, glm::mat4 value) const -> void;
        };
    };
};
