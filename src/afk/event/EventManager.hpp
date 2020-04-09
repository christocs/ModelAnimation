#pragma once

#include <cstdint>
#include <functional>
#include <queue>
#include <unordered_map>
#include <vector>

#include "afk/event/Event.hpp"

struct GLFWwindow;

namespace Afk {
  class EventManager {
    using Callback = std::function<void(Afk::Event)>;

  public:
    EventManager();

    auto pump_events() -> void;
    auto register_event(Event::Type type, Callback callback) -> void;
    auto setup_callbacks(GLFWwindow *window) -> void;

    // FIXME: Move to keyboard manager.
    std::unordered_map<Event::Action, bool> key_state = {
        {Event::Action::Forward, false},
        {Event::Action::Left, false},
        {Event::Action::Backward, false},
        {Event::Action::Right, false},
    };

  private:
    static auto key_callback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) -> void;
    static auto char_callback(GLFWwindow *window, uint32_t codepoint) -> void;
    static auto mouse_pos_callback(GLFWwindow *window, double x, double y) -> void;
    static auto mouse_press_callback(GLFWwindow *window, int button, int action,
                                     int mods) -> void;
    static auto mouse_scroll_callback(GLFWwindow *window, double dx, double dy) -> void;

    std::queue<Event> events = {};

    std::unordered_map<Event::Type, std::vector<Callback>> callbacks = {
        {Event::Type::MouseDown, {}},   {Event::Type::MouseUp, {}},
        {Event::Type::MouseMove, {}},   {Event::Type::KeyDown, {}},
        {Event::Type::KeyUp, {}},       {Event::Type::TextEnter, {}},
        {Event::Type::MouseScroll, {}}, {Event::Type::Update, {}}};
  };
}
