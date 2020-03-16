#include "afk/Afk.hpp"

#include <stdexcept>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "afk/render/Camera.hpp"
#include "afk/render/Model.hpp"
#include "afk/render/Shader.hpp"

#ifdef __APPLE__
constexpr auto depthBufferSize = 32;
#else
constexpr auto depthBufferSize = 24;
#endif

using std::runtime_error;
using std::vector;

using Movement = Afk::Render::Camera::Movement;
using Afk::Engine;
using Afk::Render::Camera;
using Afk::Render::Model;
using Afk::Render::Shader;
using glm::vec3;

// FIXME: Too tightly coupled, split off into subsystems.

Engine::Engine()
    : window(sf::Window{sf::VideoMode{1920, 1080}, "ICT397", sf::Style::Fullscreen,
                        sf::ContextSettings{depthBufferSize, 8, 4, 4, 1,
                                            sf::ContextSettings::Core}}) {

    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(true);

    if (!gladLoadGL()) {
        throw runtime_error{"Failed to initialize GLAD"};
    }

    // Set OpenGL options.
    glEnable(GL_DEPTH_TEST);

    // Load assets.
    this->shader = Shader{"shader/vertex.glsl", "shader/fragment.glsl"};
    this->models = vector<Model>{Model{"res/model/city/city.fbx"}};
    this->camera = Camera{vec3{0.0f, 50.0f, 0.0f}};

    // Scale and rotate the world.
    models[0].setScale(vec3{0.25f});
    models[0].setRotation(
        glm::angleAxis(glm::radians(-90.0f), vec3{1.0f, 0.0f, 0.0f}));
}

auto Engine::handleEvents() -> void {
    auto event = sf::Event{};

    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->isRunning = false;
        } else if (event.type == sf::Event::Resized) {
            glViewport(0, 0, static_cast<GLsizei>(event.size.width),
                       static_cast<GLsizei>(event.size.height));
        }
    }
}

auto Engine::update() -> void {
    this->handleKeys();
    this->handleMouse();
    this->lastUpdate = this->getTime();
}

auto Engine::render() -> void {
    // Calculate the new projection and view matrix.
    const auto [width, height] = this->window.getSize();
    const auto projection      = glm::perspective(
        glm::radians(this->camera.getFov()),
        static_cast<float>(width) / static_cast<float>(height), 50.0f, 10000.0f);
    const auto view = this->camera.getViewMatrix();

    // Clear the screen.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable the shader.
    shader.use();
    shader.setUniform("projection", projection);
    shader.setUniform("view", view);

    // Draw all models.
    for (const auto &model : models) {
        model.draw(this->shader);
    }

    // Swap the front and back buffer.
    this->window.display();
}

auto Engine::getDeltaTime() const -> float {
    return this->getTime() - this->lastUpdate;
}

auto Engine::getTime() const -> float {
    return this->clock.getElapsedTime().asSeconds();
}

auto Engine::getIsRunning() const -> bool {
    return this->isRunning;
}

auto Engine::handleMouse() -> void {
    static auto firstFrame = true;

    const auto [windowW, windowH] = static_cast<sf::Vector2i>(window.getSize());
    const auto centerX            = windowW / 2;
    const auto centerY            = windowH / 2;

    const auto [x, y] = static_cast<sf::Vector2i>(sf::Mouse::getPosition(window));
    const auto offsetX = x - centerX;
    const auto offsetY = centerY - y;

    if (!firstFrame) {
        this->camera.processMouse(offsetX, offsetY);
    } else {
        firstFrame = false;
    }

    sf::Mouse::setPosition(sf::Vector2i{centerX, centerY}, window);
}

auto Engine::handleKeys() -> void {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->camera.processKeys(Movement::FORWARD, this->getDeltaTime());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->camera.processKeys(Movement::LEFT, this->getDeltaTime());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->camera.processKeys(Movement::BACKWARD, this->getDeltaTime());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->camera.processKeys(Movement::RIGHT, this->getDeltaTime());
    }
}
