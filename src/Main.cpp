#include <cstddef>

#include "afk/Afk.hpp"

auto main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) -> int {
    auto afk = Afk::Engine{};

    while (afk.getIsRunning()) {
        afk.handleEvents();
        afk.update();
        afk.render();
    }

    return EXIT_SUCCESS;
}
