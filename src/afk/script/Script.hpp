#pragma once

#include <string>
#include <vector>

/**
 * Acts as a façade between events and Lua.
 */
namespace Afk {
  namespace Lua {
    struct Key {
      std::string name = {};
      int code         = {};
    };

    struct Mouse {
      std::string name = {};
      int button       = {};
    };

    struct LuaEvent {
      std::string name = {};
      int event        = {};
    };

    /**
     * Return not const as references to these values are used by lua.
     * Despite being "readonly" from lua's side, they can't be const.
     * (Even readonly variables can be changed in lua, with things like rawset.)
     */
    auto get_keys() -> std::vector<Key> &;
    auto get_buttons() -> std::vector<Mouse> &;
    auto get_events() -> std::vector<LuaEvent> &;
  }
}
