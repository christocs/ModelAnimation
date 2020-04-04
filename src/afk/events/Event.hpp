#pragma once
#include <string>
#include <variant>

namespace Afk {
  struct Event {
    struct MouseMove {
      double mouse_x;
      double mouse_y;
    };
    struct MouseButton {
      int mouse_code;
    };
    struct Key {
      int key_code;
      bool control;
      bool alt;
      bool shift;
    };
    struct Text {
      std::string text;
    };
    struct MouseScroll {
      double scroll_x;
      double scroll_y;
    };
    struct Update {
      double delta_time;
    };
    enum class EventType {
      MouseDown,
      MouseUp,
      MouseMove,
      KeyDown,
      KeyUp,
      TextEnter,
      MouseScroll,
      Update
    };
    using EventData = std::variant<MouseMove, MouseButton, Key, Text, MouseScroll, Update>;
    EventData data;
    EventType type;
  };
}
