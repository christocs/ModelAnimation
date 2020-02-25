#include <iostream>

#include <SFML/Window.hpp>

using sf::Window;

auto main() -> int {
    auto window = sf::Window{sf::VideoMode(800, 600), "ICT397"};

    while (window.isOpen()) {
        auto event = sf::Event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
}
