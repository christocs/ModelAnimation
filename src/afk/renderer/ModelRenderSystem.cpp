#include "afk/renderer/ModelRenderSystem.hpp"

#include "afk/physics/Transform.hpp"
#include "afk/io/ModelSource.hpp"

auto Afk::render_models(entt::registry* registry, Afk::Renderer* renderer, const Afk::ShaderProgramHandle* shader) -> void {
  auto render_view = registry->view<Afk::Transform, Afk::ModelSource>();

  for (auto entity: render_view) {
    auto model_name = render_view.get<Afk::ModelSource>(entity);
    auto model_transform = render_view.get<Afk::Transform>(entity);

    auto model_handle = renderer->get_model(model_name);
    renderer->draw_model(model_handle, *shader, model_transform);
  }
}

