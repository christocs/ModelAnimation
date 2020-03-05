#include "Main.hpp"

#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <SFML/Window.hpp>
#include <assimp/Importer.hpp>
#include <cpplocate/cpplocate.h>
#include <glad/glad.h>

using sf::Window;
using std::array;
using std::ifstream;
using std::runtime_error;
using std::string;
using std::vector;

constexpr auto points = array<float, 9>{0.0f,  0.5f,  0.0f,  //
                                        0.5f,  -0.5f, 0.0f,  //
                                        -0.5f, -0.5f, 0.0f}; //

auto compileShader(const string &path, GLuint type) -> GLuint {
    auto absPath = cpplocate::getModulePath() + '/' + path;
    auto file    = ifstream{absPath};

    if (!file) {
        throw runtime_error{string{"Unable to open file '"} + absPath + "'"};
    }

    const auto source = string{(std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>()} +
                        '\0';

    const auto shader      = glCreateShader(type);
    const auto shaderArray = source.c_str();
    glShaderSource(shader, 1, &shaderArray, nullptr);
    glCompileShader(shader);

    auto didSucceed = GLint{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &didSucceed);

    if (!didSucceed) {
        auto errorLength = GLint{0};
        auto errorMsg    = vector<GLchar>{};

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);
        errorMsg.resize(static_cast<size_t>(errorLength));

        glGetShaderInfoLog(shader, errorLength, &errorLength, errorMsg.data());
        glDeleteShader(shader);

        throw runtime_error{path + ": " + errorMsg.data()};
    }

    return shader;
}

auto main() -> int {
    // Setup window and OpenGL context.
    auto settings           = sf::ContextSettings{};
    settings.majorVersion   = 4;
    settings.minorVersion   = 1;
    settings.attributeFlags = sf::ContextSettings::Core;

    auto window = sf::Window{sf::VideoMode(800, 600), "ICT397",
                             sf::Style::Default, settings};

    if (!gladLoadGL()) {
        throw runtime_error{"Failed to initialize GLAD"};
    }

    // Setup VBO.
    auto vbo = GLuint{};
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(),
                 GL_STATIC_DRAW);

    // Setup VAO.
    auto vao = GLuint{};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Compile shaders.
    const auto vertexShader =
        compileShader("shader/vertex/default.glsl", GL_VERTEX_SHADER);
    const auto fragmentShader =
        compileShader("shader/fragment/default.glsl", GL_FRAGMENT_SHADER);

    // Setup shader program.
    const auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    while (window.isOpen()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.display();

        auto event = sf::Event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, static_cast<GLsizei>(event.size.width),
                           static_cast<GLsizei>(event.size.height));
            }
        }
    }
}
