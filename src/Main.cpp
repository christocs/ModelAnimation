#include <cstdlib>

#include "afk/Afk.hpp"

auto main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) -> int {
  auto &afk = Afk::Engine::get();

  auto vert = afk.renderer.get_shader("shader/default.vert");
  auto frag = afk.renderer.get_shader("shader/default.frag");
  afk.renderer.link_shaders("default", {vert, frag});

  while (afk.get_is_running()) {
    afk.update();
    afk.render();
  }

  return EXIT_SUCCESS;
}
