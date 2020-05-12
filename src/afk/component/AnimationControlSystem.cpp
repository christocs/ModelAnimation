#include "afk/component/AnimationControlSystem.hpp"

#include <glm/vec3.hpp>
#include "afk/Afk.hpp"
#include "afk/component/AnimationFrame.hpp"
#include "afk/physics/PhysicsBody.hpp"

auto Afk::AnimationControlSystem::update(entt::registry *registry, double dt) -> void {
  // advance animation time if any key is down
  if (Engine::get().event_manager.key_state[Event::Action::ForwardArrow] ||
      Engine::get().event_manager.key_state[Event::Action::BackwardArrow] ||
      Engine::get().event_manager.key_state[Event::Action::RightArrow] ||
      Engine::get().event_manager.key_state[Event::Action::LeftArrow]) {
    this->current_animation_time += dt;
  }

//  registry->view<Afk::AnimationFrame, Afk::PhysicsBody>().each(
//      [time](Afk::AnimationFrame &animation_frame, Afk::PhysicsBody &collision) {
//        // update animation's frame to the latest time
//        animation_frame.time = time;
//
//        // adjust move direction according to arrow keys
//        auto move_direction = glm::vec3(0.0f);
//        if (Engine::get().event_manager.key_state[Event::Action::ForwardArrow]) {
//          move_direction.z += 1.0f * AnimationControlSystem::move_speed;
//        }
//        if (Engine::get().event_manager.key_state[Event::Action::BackwardArrow]) {
//          move_direction.z -= 1.0f * AnimationControlSystem::move_speed;
//        }
//        if (Engine::get().event_manager.key_state[Event::Action::LeftArrow]) {
//          move_direction.x += 1.0f * AnimationControlSystem::move_speed;
//        }
//        if (Engine::get().event_manager.key_state[Event::Action::RightArrow]) {
//          move_direction.x -= 1.0f * AnimationControlSystem::move_speed;
//        }
//        collision.apply_force(move_direction);
//      });

  auto view = registry->view<Afk::AnimationFrame>();
  for (auto &entity : view) {
    view.get<Afk::AnimationFrame>(entity).time = this->current_animation_time;
  }
}
