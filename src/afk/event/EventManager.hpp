#pragma once

#include <cstdint>
#include <functional>
#include <queue>
#include <unordered_map>
#include <vector>

#include "afk/event/Event.hpp"
#include "afk/renderer/Renderer.hpp"

struct GLFWwindow;

namespace Afk {
  /**
   * Event manager
   */
  class EventManager {
  public:
    /**
     * Callback type to wrap an std::function
     * as std::function does not have operator==
     * but we need something like that to deregister events
     */
    class Callback {
    public:
      /**
       * Constructor
       * @param fn function to wrap
       */
      Callback(std::function<void(Afk::Event)> fn);
      /**
       * Comparison
       */
      auto operator==(const Callback &rhs) const -> bool;
      /**
       * Invoke operator
       */
      auto operator()(const Afk::Event &arg) const -> void;

    private:
      std::function<void(Afk::Event)> func;
      std::size_t id;
      static std::size_t index;
    };
    // using Callback = std::function<void(Afk::Event)>;

    /**
     * Constructor
     */
    EventManager()                     = default;
    EventManager(EventManager &&)      = delete;
    EventManager(const EventManager &) = delete;
    auto operator=(const EventManager &) -> EventManager & = delete;
    auto operator=(EventManager &&) -> EventManager & = delete;
    /**
     * Init the event system
     */
    auto initialize(Renderer::Window window) -> void;
    /**
     * Pump events in queue and post an update event
     */
    auto pump_events() -> void;
    /**
     * Register a callback for an event type
     */
    auto register_event(Event::Type type, Callback callback) -> void;
    /**
     * Deregister a callback for an event type
     */
    auto deregister_event(Event::Type type, Callback callback) -> void;
    /**
     * called from init, do not call
     * @todo should be private now
     */
    auto setup_callbacks(Renderer::Window window) -> void;

    /**
     * unused cruft from debugging
     * @todo get rid of this
     */
    std::unordered_map<Event::Action, bool> key_state = {
        {Event::Action::Forward, false},
        {Event::Action::Left, false},
        {Event::Action::Backward, false},
        {Event::Action::Right, false},
    };

  private:
    /**
     * GLFW key callback
     */
    static auto key_callback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) -> void;
    /**
     * GLFW text entry callback
     */
    static auto char_callback(GLFWwindow *window, uint32_t codepoint) -> void;
    /**
     * GLFW mouse move callback
     */
    static auto mouse_pos_callback(GLFWwindow *window, double x, double y) -> void;
    /**
     * GLFW mouse press callback
     */
    static auto mouse_press_callback(GLFWwindow *window, int button, int action,
                                     int mods) -> void;
    /**
     * GLFW mouse scroll callback
     */
    static auto mouse_scroll_callback(GLFWwindow *window, double dx, double dy) -> void;
    /**
     * GLFW error callback
     */
    static auto error_callback(int error, const char *msg) -> void;

    bool is_initialized      = false;
    std::queue<Event> events = {};

    std::unordered_map<Event::Type, std::vector<Callback>> callbacks = {
        {Event::Type::MouseDown, {}},   {Event::Type::MouseUp, {}},
        {Event::Type::MouseMove, {}},   {Event::Type::KeyDown, {}},
        {Event::Type::KeyUp, {}},       {Event::Type::TextEnter, {}},
        {Event::Type::MouseScroll, {}}, {Event::Type::Update, {}}};
  };
}
