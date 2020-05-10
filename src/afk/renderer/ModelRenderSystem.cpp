#include "afk/renderer/ModelRenderSystem.hpp"

#include "afk/component/AnimationFrame.hpp"
#include "afk/io/ModelSource.hpp"
#include "afk/physics/Transform.hpp"

auto Afk::queue_models(entt::registry *registry, Afk::Renderer *renderer) -> void {
  // draw normal models without animations
  auto render_view = registry->view<Afk::Transform, Afk::ModelSource>(
      entt::exclude<Afk::AnimationFrame>);
  for (const auto entity : render_view) {
    const auto model_component = render_view.get<Afk::ModelSource>(entity);
    const auto model_transform = render_view.get<Afk::Transform>(entity);
    renderer->queue_draw({model_component.name,
                          model_component.shader_program_path, model_transform});
  }

  // draw models with animations
  auto animated_render_view =
      registry->view<Afk::Transform, Afk::ModelSource, Afk::AnimationFrame>();
  for (const auto entity : animated_render_view) {
    const auto model_component = animated_render_view.get<Afk::ModelSource>(entity);
    const auto model_transform = animated_render_view.get<Afk::Transform>(entity);
    const auto model_animation_frame =
        animated_render_view.get<Afk::AnimationFrame>(entity);
    renderer->queue_draw({model_component.name, model_component.shader_program_path,
                          model_transform, model_animation_frame});
  }
}
