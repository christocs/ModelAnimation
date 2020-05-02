#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace Afk {
  /**
   * Event type
   */
  struct Event {
    /**
     * Mouse move event
     */
    struct MouseMove {
      double x = {};
      double y = {};
    };
    /**
     * Mouse click event
     */
    struct MouseButton {
      int button   = {};
      bool control = {};
      bool alt     = {};
      bool shift   = {};
    };
    /**
     * Key event
     */
    struct Key {
      int key      = {};
      int scancode = {};
      bool control = {};
      bool alt     = {};
      bool shift   = {};
    };
    /**
     * Unicode text input event
     */
    struct Text {
      uint32_t codepoint = {};
    };
    /**
     * Mouse wheel scroll event
     */
    struct MouseScroll {
      double x = {};
      double y = {};
    };
    /**
     * Update event
     */
    struct Update {
      float dt = {};
    };

    // FIXME: Move to keyboard  handler.
    /**
     * todo: remove this
     */
    enum class Action { Forward, Backward, Left, Right };

    /**
     * Event type enum
     */
    enum class Type {
      MouseDown,
      MouseUp,
      MouseMove,
      KeyDown,
      KeyUp,
      KeyRepeat,
      TextEnter,
      MouseScroll,
      Update
    };
    /**
     * Event data variant type
     */
    using Data =
        std::variant<std::monostate, MouseMove, MouseButton, Key, Text, MouseScroll, Update>;

    /**
     * gets this as the variant type
     * @todo move to bindings.lua as a member function proxy
     */
    template<typename T>
    auto get() -> T {
      return std::get<T>(this->data);
    }

    /**
     * event variant data
     */
    Data data = {};
    /**
     * type of the event
     */
    Type type = {};
  };
}
