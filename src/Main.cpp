#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "afk/Afk.hpp"

using std::exception;

auto main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) -> int {
  auto &afk = Afk::Engine::get();

  try {
    afk.initialize();

    while (afk.get_is_running()) {
      afk.update();
      afk.render();
    }
  } catch (const exception &e) {
    std::cerr << e.what();
  }

  return EXIT_SUCCESS;
}
