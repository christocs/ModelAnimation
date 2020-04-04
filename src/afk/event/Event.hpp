#pragma once
#include <string>
#include <variant>

namespace Afk {
  struct Event {
    struct MouseMove {
      double x = {};
      double y = {};
    };

    struct MouseButton {
      int mouse_code = {};
    };

    struct Key {
      int key_code = {};
      bool control = {};
      bool alt     = {};
      bool shift   = {};
    };

    struct Text {
      std::string text;
    };

    struct MouseScroll {
      double x = {};
      double y = {};
    };

    struct Update {
      float dt = {};
    };

    // FIXME: Move to keyboard  handler.
    enum class Action { Forward, Backward, Left, Right };

    enum class Type {
      MouseDown,
      MouseUp,
      MouseMove,
      KeyDown,
      KeyUp,
      TextEnter,
      MouseScroll,
      Update
    };

    using Data = std::variant<MouseMove, MouseButton, Key, Text, MouseScroll, Update>;

    Data data = {};
    Type type = {};
  };
}
