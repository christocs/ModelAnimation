#pragma once

#include <cstdint>
#include <map>
#include <queue>
#include <vector>

#include "Callback.hpp"
#include "Event.hpp"

struct GLFWwindow;
namespace Afk {
  class EventManager {
  public:
    EventManager();

    auto pump_events() -> void;
    auto register_event(Event::EventType type, Callback callback) -> void;
    auto setup_callbacks(GLFWwindow *win) -> void;

  private:
    static auto key_callback(GLFWwindow *win, int key, int scancode, int action,
                             int mods) -> void;
    static auto char_callback(GLFWwindow *win, uint32_t codepoint) -> void;
    static auto mouse_pos_callback(GLFWwindow *win, double xpos, double ypos) -> void;
    static auto mouse_press_callback(GLFWwindow *win, int btn, int action, int mods) -> void;
    static auto mouse_scroll_callback(GLFWwindow *win, double xdelta, double ydelta) -> void;

    std::queue<Event> events                                    = {};
    std::map<Event::EventType, std::vector<Callback>> callbacks = {
        {Event::EventType::MouseDown, {}},   {Event::EventType::MouseUp, {}},
        {Event::EventType::MouseMove, {}},   {Event::EventType::KeyDown, {}},
        {Event::EventType::KeyUp, {}},       {Event::EventType::TextEnter, {}},
        {Event::EventType::MouseScroll, {}}, {Event::EventType::Update, {}}};
  };
}
