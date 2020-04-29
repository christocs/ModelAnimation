#include "LuaScript.hpp"

#include "afk/debug/Assert.hpp"
#include "afk/event/EventManager.hpp"
#include "afk/io/Path.hpp"
#include "afk/script/Script.hpp"

using namespace std::string_literals;
using Afk::LuaScript;

template<typename VariantType, typename VariantReturn, typename LuabridgeScope>
auto luabridge_add_variant(const char *name, LuabridgeScope &b) -> void {
  b.addStaticCFunction(name, static_cast<VariantReturn (*)(VariantType)>(
                                 std::get<VariantReturn>)
}

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

  auto afk_event_class =
      luabridge::getGlobalNamespace(lua).beginClass<Afk::Event>("event");
  for (const auto &event : Afk::Script::events) {
    afk_event_class.addStaticData<int>(event.name.c_str(),
                                       const_cast<int *>(&event.type), false);
  }
  auto events_namespace =
      luabridge::getGlobalNamespace(lua).beginNamespace("events");

  events_namespace.beginClass<Afk::Event::Key>("key")
      .addData("alt", &Afk::Event::Key::alt, false)
      .addData("control", &Afk::Event::Key::control, false)
      .addData("shift", &Afk::Event::Key::shift, false)
      .addData("key", &Afk::Event::Key::key, false)
      .addData("keycode", &Afk::Event::Key::scancode, false)
      .endClass();
  luabridge_add_variant<Afk::Event::Data, Afk::Event::Key>("to_key", afk_event_class);

  events_namespace.beginClass<Afk::Event::MouseButton>("mouse_button")
      .addData("alt", &Afk::Event::MouseButton::alt, false)
      .addData("control", &Afk::Event::MouseButton::control, false)
      .addData("shift", &Afk::Event::MouseButton::shift, false)
      .addData("button", &Afk::Event::MouseButton::button, false)
      .endClass();
  luabridge_add_variant<Afk::Event::Data, Afk::Event::MouseButton>(
      "to_mouse_button", afk_event_class);

  events_namespace.beginClass<Afk::Event::MouseButton>("mouse_button")
      .addData("alt", &Afk::Event::MouseButton::alt, false)
      .addData("control", &Afk::Event::MouseButton::control, false)
      .addData("shift", &Afk::Event::MouseButton::shift, false)
      .addData("button", &Afk::Event::MouseButton::button, false)
      .endClass();
  luabridge_add_variant<Afk::Event::Data, Afk::Event::MouseMove>(
      "to_mouse_move", afk_event_class);

  events_namespace.beginClass<Afk::Event::MouseScroll>("mouse_scroll")
      .addData("x", &Afk::Event::MouseScroll::x, false)
      .addData("y", &Afk::Event::MouseScroll::y, false)
      .endClass();
  luabridge_add_variant<Afk::Event::Data, Afk::Event::MouseScroll>(
      "to_mouse_scroll", afk_event_class);

  events_namespace.beginClass<Afk::Event::Text>("text")
      .addData("codepoint", &Afk::Event::Text::codepoint, false)
      .endClass();
  luabridge_add_variant<Afk::Event::Data, Afk::Event::Text>("to_text", afk_event_class);

  events_namespace.beginClass<Afk::Event::Update>("update")
      .addData("delta", &Afk::Event::Update::dt, false)
      .endClass();
  luabridge_add_variant<Afk::Event::Data, Afk::Event::Update>("to_update", afk_event_class);
  afk_event_class.endClass();

  auto script_class = luabridge::getGlobalNamespace(lua).beginClass<LuaScript>("script");
  script_class.addFunction("register_event", &Afk::LuaScript::register_fn);
  script_class.endClass();
}

auto LuaScript::register_fn(Afk::Event::Type event_type, LuaRef func) -> void {
  this->registered_events.push_back(RegisteredLuaCall{
      event_type, Afk::EventManager::Callback{std::function<void(Afk::Event)>{func}}});
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

LuaScript::LuaScript(Afk::EventManager *events)
  : event_manager(events), registered_events() {
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
