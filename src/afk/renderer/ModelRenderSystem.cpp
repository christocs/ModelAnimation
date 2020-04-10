#include "afk/renderer/ModelRenderSystem.hpp"

#include "afk/physics/Transform.hpp"
#include "afk/io/ModelSource.hpp"

auto Afk::render_models(entt::registry* registry, Afk::Renderer* renderer, const Afk::ShaderProgramHandle* shader) -> void {
  auto renderView = registry->view<Afk::Transform, Afk::ModelSource>();

  for (auto entity: renderView) {
    auto modelName = renderView.get<Afk::ModelSource>(entity);
    auto modelTransform = renderView.get<Afk::Transform>(entity);

    auto modelHandle = renderer->get_model(modelName);
    renderer->draw_model(modelHandle, *shader, modelTransform);
  }
}

