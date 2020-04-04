#pragma once

#include <string>
#include <vector>

/**
 * Acts as a façade between events and Lua.
 */
namespace Afk {
  class Script {
  public:
    struct Event {
      std::string name = {};
      int type         = {};
    };

    struct Mouse {
      std::string name = {};
      int button       = {};
    };

    struct Key {
      std::string name = {};
      int code         = {};
    };

    static const std::vector<Event> events;
    static const std::vector<Mouse> mouse_buttons;
    static const std::vector<Key> keys;
  };
}
