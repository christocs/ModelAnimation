#include <cstdlib>

#include "afk/Afk.hpp"
#include "afk/render/ModelData.hpp"

auto main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) -> int {
  auto afk = Afk::Engine{};

  auto vert = afk.renderer.getShader("shader/default.vert");
  auto frag = afk.renderer.getShader("shader/default.frag");
  afk.renderer.linkShaders("default", {vert, frag});

  while (afk.getIsRunning()) {
    afk.update();
    afk.render();
  }

  return EXIT_SUCCESS;
}
