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

#ifdef __APPLE__
constexpr auto depthBufferSize = 32;
#else
constexpr auto depthBufferSize = 24;
#endif

auto handleMouse(Camera &camera, const sf::Window &window) -> void;
auto handleKeys(Camera &camera, float deltaTime) -> void;

auto handleMouse(Camera &camera, const sf::Window &window) -> void {
    static auto firstFrame = true;

    const auto [windowW, windowH] = static_cast<sf::Vector2i>(window.getSize());
    const auto centerX            = windowW / 2;
    const auto centerY            = windowH / 2;

    const auto [x, y] = static_cast<sf::Vector2i>(sf::Mouse::getPosition(window));
    const auto offsetX = x - centerX;
    const auto offsetY = centerY - y;

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
        sf::Window{sf::VideoMode{1920, 1080}, "ICT397", sf::Style::Fullscreen,
                   sf::ContextSettings{depthBufferSize, 8, 4, 4, 1,
                                       sf::ContextSettings::Core}};

    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(true);

    if (!gladLoadGL()) {
        throw runtime_error{"Failed to initialize GLAD"};
    }

    // Set OpenGL options.
    glEnable(GL_DEPTH_TEST);

    // Load assets.
    auto shader = Shader{"shader/vertex.glsl", "shader/fragment.glsl"};
    auto models = vector<Model>{Model{"res/city/city.fbx"}};

    // Setup camera.
    auto camera = Camera{vec3{0.0f, 50.0f, 0.0f}};

    // Scale and rotate the world.
    models[0].setScale(vec3{0.25f});
    models[0].setRotation(
        glm::angleAxis(glm::radians(-90.0f), vec3{1.0f, 0.0f, 0.0f}));

    auto clock    = sf::Clock{};
    auto lastTime = clock.getElapsedTime().asSeconds();

    while (window.isOpen()) {
        auto event = sf::Event{};

        // Poll events.
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

        handleKeys(camera, deltaTime);
        handleMouse(camera, window);

        // Calculate the new projection and view matrix.
        const auto [width, height] = window.getSize();
        const auto projection = glm::perspective(glm::radians(camera.getFov()),
                                                 static_cast<float>(width) /
                                                     static_cast<float>(height),
                                                 100.0f, 10000.0f);
        const auto view       = camera.getViewMatrix();

        // Clear the screen.
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable the shader.
        shader.use();
        shader.setUniform("projection", projection);
        shader.setUniform("view", view);

        // Draw all models.
        for (auto &i : models) {
            i.draw(shader);
        }

        // Swap the front and back buffer.
        window.display();
    }

    return 0;
}
