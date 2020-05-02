#pragma once

struct lua_State;
namespace Afk {
  /**
   * Add the various bindings to the engine
   * See external lua documentation to see what is added
   */
  auto add_engine_bindings(lua_State *l) -> void;
}
