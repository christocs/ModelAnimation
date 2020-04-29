#include "LuaScript.hpp"

#include "afk/debug/Assert.hpp"
#include "afk/event/EventManager.hpp"
#include "afk/io/Path.hpp"
#include "afk/script/Script.hpp"

using namespace std::string_literals;
using Afk::LuaScript;

/**
 * This should probably be moved somewhere better
 */
auto LuaScript::setup_lua_state(lua_State *lua) -> void {
  auto key_ns = luabridge::getGlobalNamespace(lua).beginNamespace("key");
  for (const auto &key : Afk::Script::keys) {
    // key.code can't be changed from lua's side
    key_ns.addVariable<int>(key.name.c_str(), const_cast<int *>(&key.code), false);
  }
  key_ns.endNamespace();

  auto mouse_ns = luabridge::getGlobalNamespace(lua).beginNamespace("mouse");
  for (const auto &mouse_button : Afk::Script::mouse_buttons) {
    mouse_ns.addVariable<int>(mouse_button.name.c_str(),
                              const_cast<int *>(&mouse_button.button), false);
  }
  mouse_ns.endNamespace();

  auto script_class = luabridge::getGlobalNamespace(lua).beginClass<LuaScript>("script");
  for (const auto &event : Afk::Script::events) {
    script_class.addStaticProperty<int>(event.name.c_str(),
                                        const_cast<int *>(&event.type), false);
  }

  // TODO: Think of a way to allow live reloading with this registration type
  // (Right now when you reload the file I think old registered events will stay
  // around) (Maybe need some alternate register function that lua will use)
  script_class.addFunction("register_event", &Afk::LuaScript::register_fn);
  script_class.endClass();
}

auto LuaScript::register_fn(Afk::Event::Type event_type, LuaRef func) -> void {
  this->registered_events.push_back(
      RegisteredLuaCall{event_type, Afk::EventManager::Callback{func}});
  auto &callback = this->registered_events.at(this->registered_events.size() - 1);
  event_manager->register_event(event_type, callback.lua_ref);
}

auto LuaScript::load(const std::filesystem::path &filename, lua_State *lua) -> void {
  this->unload();
  const auto abs_path = Afk::get_absolute_path(filename);
  luabridge::getGlobalNamespace(lua).addVariable("this", this);
  afk_assert(luaL_dofile(lua, abs_path.string().c_str()) != 0,
             "Error loading "s + filename.string() + ": "s + lua_tostring(lua, -1));
}

auto LuaScript::unload() -> void {
  while (this->registered_events.size() > 0) {
    auto &callback = this->registered_events[this->registered_events.size() - 1];
    event_manager->deregister_event(callback.event_type, callback.lua_ref);
    this->registered_events.erase(this->registered_events.end() - 1);
  }
}

LuaScript::LuaScript(Afk::EventManager *event_manager)
  : event_manager(event_manager), registered_events() {
  afk_assert(event_manager != nullptr, "Event manager must not be null.");
}
LuaScript::~LuaScript() {
  this->unload();
}
