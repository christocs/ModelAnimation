#include <iostream>
#include <reactphysics3d.h>

#include <SFML/Window.hpp>
#include <assimp/Importer.hpp>
#include <entt/entt.hpp>

using sf::Window;

using namespace reactphysics3d;

auto main() -> int {
    auto window       = sf::Window{sf::VideoMode(800, 600), "ICT397"};
    auto importer     = Assimp::Importer{};
    auto *sphereShape = new SphereShape(10.0f);

    while (window.isOpen()) {
        auto event = sf::Event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
}
