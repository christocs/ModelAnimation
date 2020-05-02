#pragma once

#include <filesystem>
#include <string>
extern "C" {
#include <lua/lauxlib.h>
#include <lua/lua.h>
#include <lua/lualib.h>
}
#include <LuaBridge/LuaBridge.h>

#include "afk/event/Event.hpp"
#include "afk/event/EventManager.hpp"

using luabridge::LuaRef;

namespace Afk {
  /**
   * A lua function that has been registered to the event manager
   * The lua script needs to remember what has been registered
   * so it can deregister them when the script is unloaded.
   */
  struct RegisteredLuaCall {
    Afk::Event::Type event_type;
    Afk::EventManager::Callback callback;
  };

  /**
   * Internal lua script class, to be managed by the script component.
   */
  class LuaScript {
  public:
    /**
     * Constructor
     */
    LuaScript(EventManager *events);
    /**
     * Copy constructor (required for luabridge)
     */
    LuaScript(const LuaScript &other);
    /**
     * Move constructor
     */
    LuaScript(LuaScript &&other);
    /**
     * Copy assignment (required for luabridge)
     */
    auto operator=(const LuaScript &other) -> LuaScript &;
    /**
     * Move assignment
     */
    auto operator=(LuaScript &&other) -> LuaScript &;
    /**
     * Destructor
     */
    ~LuaScript();
    /**
     * Loads a script from the path and lua state
     * @param filename script to load
     * @param lua lua state to use (typically Afk::Engine::get().lua)
     */
    auto load(const std::filesystem::path &filename, lua_State *lua) -> void;
    /**
     * Unloads the script, deregistering all the events in it
     */
    auto unload() -> void;
    /**
     * Allows script to register its functions into the event system
     */
    auto register_fn(int event_val, LuaRef func) -> void;

  private:
    /**
     * ptr to the event manager
     */
    EventManager *event_manager;
    /**
     * currently registered events
     */
    std::shared_ptr<std::vector<RegisteredLuaCall>> registered_events;
  };
}
