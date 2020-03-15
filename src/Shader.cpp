#include "Shader.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

#include <cpplocate/cpplocate.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.hpp"

using namespace std::string_literals;
using glm::mat4;
using std::ifstream;
using std::istreambuf_iterator;
using std::runtime_error;
using std::string;
using std::vector;

auto Shader::compileShader(const string &path, GLuint type) -> GLuint {
    const auto absPath = cpplocate::getModulePath() + '/' + path;
    auto file          = ifstream{absPath};

    if (!file) {
        throw runtime_error{"Unable to open shader '"s + absPath + "'"s};
    }

    const auto source =
        string{(istreambuf_iterator<char>(file)), istreambuf_iterator<char>()} + '\0';

    const auto shader         = glCreateShader(type);
    const auto *shaderPointer = source.c_str();
    glShaderSource(shader, 1, &shaderPointer, nullptr);
    glCompileShader(shader);

    auto didSucceed = GLint{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &didSucceed);

    if (!didSucceed) {
        auto errorLength = GLint{0};
        auto errorMsg    = vector<GLchar>{};

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);
        errorMsg.resize(static_cast<size_t>(errorLength));
        glGetShaderInfoLog(shader, errorLength, &errorLength, errorMsg.data());

        throw runtime_error{"Shader compilation failed: " + path + ": " +
                            errorMsg.data()};
    }

    return shader;
}

auto Shader::linkShader(GLuint vertexShader, GLuint fragmentShader) -> GLuint {
    const auto program = glCreateProgram();

    glAttachShader(program, fragmentShader);
    glAttachShader(program, vertexShader);
    glLinkProgram(program);

    auto didSucceed = GLint{};
    glGetProgramiv(program, GL_LINK_STATUS, &didSucceed);

    if (!didSucceed) {
        auto errorLength = GLint{0};
        auto errorMsg    = vector<GLchar>{};

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorLength);
        errorMsg.resize(static_cast<size_t>(errorLength));
        glGetProgramInfoLog(program, errorLength, &errorLength, errorMsg.data());

        throw runtime_error{string{"Shader linking failed: "} + errorMsg.data()};
    }

    return program;
}

Shader::Shader(const string &vertexPath, const string &fragmentPath) {
    const auto vertex   = this->compileShader(vertexPath, GL_VERTEX_SHADER);
    const auto fragment = this->compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    this->id            = this->linkShader(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

auto Shader::use() const -> void {
    glUseProgram(this->id);
}

auto Shader::setUniform(const string &name, bool value) const -> void {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()),
                static_cast<GLboolean>(value));
}

auto Shader::setUniform(const string &name, int value) const -> void {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()),
                static_cast<GLint>(value));
}

auto Shader::setUniform(const string &name, unsigned value) const -> void {
    glUniform1ui(glGetUniformLocation(this->id, name.c_str()),
                 static_cast<GLuint>(value));
}

auto Shader::setUniform(const string &name, float value) const -> void {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()),
                static_cast<GLfloat>(value));
}

auto Shader::setUniform(const string &name, mat4 value) const -> void {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1,
                       GL_FALSE, glm::value_ptr(value));
}

auto Shader::getId() const -> GLuint {
    return this->id;
}
