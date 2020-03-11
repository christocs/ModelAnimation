#include <iostream>
#include <stdexcept>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Model.hpp"
#include "Shader.hpp"

using glm::mat4;
using glm::vec3;
using std::runtime_error;
using std::vector;

auto handleMouse(Camera &camera, const sf::Window &window) -> void;
auto handleKeys(Camera &camera, float deltaTime) -> void;

auto handleMouse(Camera &camera, const sf::Window &window) -> void {
    static auto firstFrame = true;

    const auto [windowW, windowH] = window.getSize();
    const auto centerX            = windowW / 2;
    const auto centerY            = windowH / 2;

    const auto [x, y]  = sf::Mouse::getPosition(window);
    const auto offsetX = static_cast<int>(x) - static_cast<int>(centerX);
    const auto offsetY = static_cast<int>(centerY) - static_cast<int>(y);

    if (!firstFrame) {
        camera.processMouse(offsetX, offsetY);
    } else {
        firstFrame = false;
    }

    sf::Mouse::setPosition(
        sf::Vector2i{static_cast<int>(centerX), static_cast<int>(centerY)}, window);
}

auto handleKeys(Camera &camera, float deltaTime) -> void {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        camera.processKeys(FORWARD, deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        camera.processKeys(LEFT, deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        camera.processKeys(BACKWARD, deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        camera.processKeys(RIGHT, deltaTime);
    }
}

auto main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) -> int {
    // Setup window and OpenGL context.
    auto window =
        sf::Window{sf::VideoMode{1920, 1080}, "ICT397", sf::Style::Default,
                   sf::ContextSettings{32, 8, 4, 4, 1, sf::ContextSettings::Core}};

    window.setMouseCursorVisible(false);

    if (!gladLoadGL()) {
        throw runtime_error{"Failed to initialize GLAD"};
    }

    glEnable(GL_DEPTH_TEST);

    auto camera = Camera{glm::vec3{0.0f, 0.0f, 3.0f}};
    auto shader = Shader{"shader/model.vs", "shader/model.fs"};
    auto models = vector<Model>{Model{"res/nanosuit/nanosuit.obj"}};

    auto clock    = sf::Clock{};
    auto lastTime = clock.getElapsedTime().asSeconds();

    while (window.isOpen()) {
        auto event = sf::Event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, static_cast<GLsizei>(event.size.width),
                           static_cast<GLsizei>(event.size.height));
            }
        }

        const auto deltaTime = clock.getElapsedTime().asSeconds() - lastTime;
        lastTime             = clock.getElapsedTime().asSeconds();

        std::cout << camera.position.x << '\t' << camera.position.y << '\t'
                  << camera.position.z << '\n';

        handleKeys(camera, deltaTime);
        handleMouse(camera, window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        const auto [width, height] = window.getSize();
        const auto projection      = glm::perspective(
            glm::radians(camera.getFov()),
            static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
        const auto view = camera.getViewMatrix();

        shader.setUniform("projection", projection);
        shader.setUniform("view", view);

        auto model = mat4(1.0f);
        model      = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model      = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

        shader.setUniform("model", model);

        for (auto &i : models) {
            i.draw(shader);
        }

        window.display();
    }

    return 0;
}
