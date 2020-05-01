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

  luabridge::getGlobalNamespace(lua)
      .beginNamespace("events")
      .beginClass<Afk::Event::Key>("key")
      .addData("alt", &Afk::Event::Key::alt, false)
      .addData("control", &Afk::Event::Key::control, false)
      .addData("shift", &Afk::Event::Key::shift, false)
      .addData("key", &Afk::Event::Key::key, false)
      .addData("key_code", &Afk::Event::Key::scancode, false)
      .endClass()
      .beginClass<Afk::Event::MouseMove>("mouse_move")
      .addData("x", &Afk::Event::MouseMove::x, false)
      .addData("y", &Afk::Event::MouseMove::y, false)
      .endClass()
      .beginClass<Afk::Event::MouseButton>("mouse_button")
      .addData("alt", &Afk::Event::MouseButton::alt, false)
      .addData("control", &Afk::Event::MouseButton::control, false)
      .addData("shift", &Afk::Event::MouseButton::shift, false)
      .addData("button", &Afk::Event::MouseButton::button, false)
      .endClass()
      .beginClass<Afk::Event::MouseScroll>("mouse_scroll")
      .addData("x", &Afk::Event::MouseScroll::x, false)
      .addData("y", &Afk::Event::MouseScroll::y, false)
      .endClass()
      .beginClass<Afk::Event::Text>("text")
      .addData("codepoint", &Afk::Event::Text::codepoint, false)
      .endClass()
      .beginClass<Afk::Event::Update>("update")
      .addData("delta", &Afk::Event::Update::dt, false)
      .endClass()
      .endNamespace();

  auto afk_event_class =
      luabridge::getGlobalNamespace(lua).beginClass<Afk::Event>("event");
  for (const auto &event : Afk::Script::events) {
    afk_event_class.addStaticData<int>(event.name.c_str(),
                                       const_cast<int *>(&event.type), false);
  }
  afk_event_class.addFunction("to_key", &Afk::Event::get<Afk::Event::Key>);
  afk_event_class.addFunction("to_mouse_move", &Afk::Event::get<Afk::Event::MouseMove>);
  afk_event_class.addFunction("to_mouse_button", &Afk::Event::get<Afk::Event::MouseButton>);
  afk_event_class.addFunction("to_mouse_scroll", &Afk::Event::get<Afk::Event::MouseScroll>);
  afk_event_class.addFunction("to_text", &Afk::Event::get<Afk::Event::Text>);
  afk_event_class.addFunction("to_update", &Afk::Event::get<Afk::Event::Update>);
  afk_event_class.endClass();

  auto script_class = luabridge::getGlobalNamespace(lua)
                          .beginClass<LuaScript>("script")
                          .addFunction("register_event", &Afk::LuaScript::register_fn)
                          .endClass();
}

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
