#include "LuaScript.hpp"

#include "afk/debug/Assert.hpp"
#include "afk/event/EventManager.hpp"
#include "afk/io/Path.hpp"
#include "afk/script/Script.hpp"

using namespace std::string_literals;
using Afk::LuaScript;

auto LuaScript::register_fn(int event_val, LuaRef func) -> void {
  auto event_type = static_cast<Afk::Event::Type>(event_val);
  this->registered_events->push_back(RegisteredLuaCall{
      event_type, Afk::EventManager::Callback{std::function<void(Afk::Event)>{func}}});
  auto &evt = this->registered_events->at(this->registered_events->size() - 1);
  event_manager->register_event(event_type, evt.callback);
}

auto LuaScript::load(const std::filesystem::path &filename, lua_State *lua) -> void {
  this->unload();
  luabridge::setGlobal(lua, this, "this");
  auto lua_ret = luaL_dofile(lua, filename.string().c_str());
  if (lua_ret != 0) {
    throw std::runtime_error{"Error loading "s + filename.string() + ": "s +
                             lua_tostring(lua, -1)};
  }
}

auto LuaScript::unload() -> void {
  while (this->registered_events != nullptr && this->registered_events->size() > 0) {
    auto &evt = this->registered_events->at(this->registered_events->size() - 1);
    event_manager->deregister_event(evt.event_type, evt.callback);
    this->registered_events->erase(this->registered_events->end() - 1);
  }
}

LuaScript::LuaScript(Afk::EventManager *events)
  : event_manager(events),
    registered_events(std::make_shared<std::vector<Afk::RegisteredLuaCall>>()) {
  afk_assert(event_manager != nullptr, "Event manager must not be null.");
}
LuaScript::~LuaScript() {
  this->unload();
}

auto LuaScript::operator=(LuaScript &&other) -> LuaScript & {
  this->event_manager     = other.event_manager;
  this->registered_events = std::move(other.registered_events);
  return *this;
}
LuaScript::LuaScript(LuaScript &&other) {
  this->event_manager     = other.event_manager;
  this->registered_events = std::move(other.registered_events);
}

auto LuaScript::operator=(const LuaScript &other) -> LuaScript & {
  this->event_manager     = other.event_manager;
  this->registered_events = other.registered_events;
  return *this;
}
LuaScript::LuaScript(const LuaScript &other) {
  this->event_manager     = other.event_manager;
  this->registered_events = other.registered_events;
}
