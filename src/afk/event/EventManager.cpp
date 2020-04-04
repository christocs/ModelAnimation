#include "afk/event/EventManager.hpp"

#include "afk/Afk.hpp"

// Must be included after GLAD.
#include <GLFW/glfw3.h>

using Afk::EventManager;
using Action = Afk::Event::Action;

auto EventManager::pump_events() -> void {
  glfwPollEvents();
  this->events.push({Event::Update{Afk::Engine::get().get_delta_time()}, Event::Type::Update});

  while (this->events.size() > 0) {
    const auto &current_event = this->events.front();

    for (const auto &event_callback : this->callbacks[current_event.type]) {
      event_callback(current_event);
    }

    this->events.pop();
  }
}

auto EventManager::register_event(Event::Type type, Callback callback) -> void {
  this->callbacks[type].push_back(callback);
}

auto EventManager::setup_callbacks(GLFWwindow *window) -> void {
  glfwSetKeyCallback(window, key_callback);
  glfwSetCharCallback(window, char_callback);
  glfwSetCursorPosCallback(window, mouse_pos_callback);
  glfwSetMouseButtonCallback(window, mouse_press_callback);
  glfwSetScrollCallback(window, mouse_scroll_callback);
}

auto EventManager::key_callback([[maybe_unused]] GLFWwindow *window, int key,
                                [[maybe_unused]] int scancode, int action, int mods) -> void {
  if (action == GLFW_REPEAT) {
    return;
  }

  auto &afk = Engine::get();

  const auto control = (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL;
  const auto alt     = (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT;
  const auto shift   = (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT;

  afk.event_manager.events.push(
      {Event::Key{key, control, alt, shift},
       action == GLFW_PRESS ? Event::Type::KeyDown : Event::Type::KeyUp});

  // FIXME: Move to keyboard manager.
  const auto new_state = action == GLFW_PRESS ? true : false;

  switch (key) {
    case GLFW_KEY_W:
      afk.event_manager.key_state[Action::Forward] = new_state;
      break;
    case GLFW_KEY_A:
      afk.event_manager.key_state[Action::Left] = new_state;
      break;
    case GLFW_KEY_S:
      afk.event_manager.key_state[Action::Backward] = new_state;
      break;
    case GLFW_KEY_D:
      afk.event_manager.key_state[Action::Right] = new_state;
      break;
  }
}

auto EventManager::char_callback([[maybe_unused]] GLFWwindow *window,
                                 [[maybe_unused]] uint32_t codepoint) -> void {
  // FIXME: Implement this
  // Afk::Engine::get().event_manager.events.push(
  //     {Event::Text{std::string{""}}, Event::Type::TextEnter});
}

auto EventManager::mouse_pos_callback([[maybe_unused]] GLFWwindow *window,
                                      double x, double y) -> void {
  Afk::Engine::get().event_manager.events.push({Event::MouseMove{x, y}, Event::Type::MouseMove});
}

auto EventManager::mouse_press_callback([[maybe_unused]] GLFWwindow *window, int button,
                                        int action, [[maybe_unused]] int mods) -> void {
  Afk::Engine::get().event_manager.events.push(
      {Event::MouseButton{button},
       action == GLFW_PRESS ? Event::Type::MouseDown : Event::Type::MouseUp});
}

auto EventManager::mouse_scroll_callback([[maybe_unused]] GLFWwindow *window,
                                         double dx, double dy) -> void {
  Afk::Engine::get().event_manager.events.push(
      {Event::MouseScroll{dx, dy}, Event::Type::MouseScroll});
}
