#include "Events.hpp"

// dont move
#include "afk/Afk.hpp"
// dont move
#include <GLFW/glfw3.h>

using Afk::EventManager;

EventManager::EventManager() {}

auto EventManager::pump_events() -> void {
  glfwPollEvents();
  this->events.push({Event::Update{Afk::Engine::get().get_delta_time()},
                     Event::EventType::Update});
  while (this->events.size() > 0) {
    const auto &current_event = this->events.front();
    for (auto &event_callback : this->callbacks[current_event.type]) {
      event_callback(current_event);
    }
    this->events.pop();
  }
}
auto EventManager::register_event(Event::EventType type, Callback callback) -> void {
  this->callbacks[type].push_back(callback);
}

auto EventManager::setup_callbacks(GLFWwindow *win) -> void {
  glfwSetKeyCallback(win, key_callback);
  glfwSetCharCallback(win, char_callback);
  glfwSetCursorPosCallback(win, mouse_pos_callback);
  glfwSetMouseButtonCallback(win, mouse_press_callback);
  glfwSetScrollCallback(win, mouse_scroll_callback);
}

auto EventManager::key_callback([[maybe_unused]] GLFWwindow *win, int key,
                                [[maybe_unused]] int scancode, int action, int mods) -> void {
  if (action == GLFW_REPEAT) {
    return;
  }
  auto control = (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL;
  auto alt     = (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT;
  auto shift   = (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT;
  Afk::Engine::get().events.events.push(
      {Event::Key{key, control, alt, shift},
       action == GLFW_PRESS ? Event::EventType::KeyDown : Event::EventType::KeyUp});
}
auto EventManager::char_callback([[maybe_unused]] GLFWwindow *win,
                                 [[maybe_unused]] uint32_t codepoint) -> void {
  throw "not implemented";
  // Afk::Engine::get().events.events.push(
  //     {Event::Text{std::string{""}}, Event::EventType::TextEnter});
}
auto EventManager::mouse_pos_callback([[maybe_unused]] GLFWwindow *win,
                                      double xpos, double ypos) -> void {
  Afk::Engine::get().events.events.push(
      {Event::MouseMove{xpos, ypos}, Event::EventType::MouseMove});
}
auto EventManager::mouse_press_callback([[maybe_unused]] GLFWwindow *win, int btn,
                                        int action, [[maybe_unused]] int mods) -> void {
  Afk::Engine::get().events.events.push(
      {Event::MouseButton{btn}, action == GLFW_PRESS ? Event::EventType::MouseDown
                                                     : Event::EventType::MouseUp});
}
auto EventManager::mouse_scroll_callback([[maybe_unused]] GLFWwindow *win,
                                         double xdelta, double ydelta) -> void {
  Afk::Engine::get().events.events.push(
      {Event::MouseScroll{xdelta, ydelta}, Event::EventType::MouseScroll});
}
