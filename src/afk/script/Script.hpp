#pragma once

#include <string>
#include <vector>

namespace Afk {
  /**
   * Stores a list of stuff to get defined in lua.
   * (eg key names, mouse button names, event names, etc)
   */
  class Script {
  public:
    /**
     * Event type names
     */
    struct Event {
      std::string name = {};
      int type         = {};
    };
    /**
     * Mouse button names
     */
    struct Mouse {
      std::string name = {};
      int button       = {};
    };
    /**
     * Key names
     */
    struct Key {
      std::string name = {};
      int code         = {};
    };

    static const std::vector<Event> events;
    static const std::vector<Mouse> mouse_buttons;
    static const std::vector<Key> keys;
  };
}
