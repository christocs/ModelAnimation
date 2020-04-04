#include "ScriptComponent.hpp"

#include <stdexcept>

#include "ScriptSetup.hpp"
#include "afk/events/Events.hpp"
#include "afk/io/Path.hpp"

/**
 * This should probably be moved somewhere better
 */
auto Afk::ScriptComponent::setup_lua_state(lua_State *lua) -> void {
  auto keyns = luabridge::getGlobalNamespace(lua).beginNamespace("key");
  for (auto &key : Afk::Lua::get_keys()) {
    // key.code can't be changed from lua's side
    keyns.addVariable<int>(key.name.c_str(), &key.code, false);
  }
  keyns.endNamespace();
  auto mousens = luabridge::getGlobalNamespace(lua).beginNamespace("mouse");
  for (auto &btn : Afk::Lua::get_buttons()) {
    mousens.addVariable<int>(btn.name.c_str(), &btn.button, false);
  }
  mousens.endNamespace();

  auto evtmgr =
      luabridge::getGlobalNamespace(lua).beginClass<EventManager>("events");
  for (auto &evt : Afk::Lua::get_events()) {
    evtmgr.addStaticProperty<int>(evt.name.c_str(), &evt.event, false);
  }
  // TODO: Think of a way to allow live reloading with this registration type
  // (Right now when you reload the file I think old registered events will stay around)
  // (Maybe need some alternate register function that lua will use)
  // evtmgr.addFunction("register", &Afk::EventManager::register_event);
  evtmgr.endClass();
}

Afk::ScriptComponent::ScriptComponent(lua_State *lua, const std::string &filename)
  : script_path(Afk::Path::get_absolute_path("script/" + filename)), on_update(lua),
    on_key_press(lua), on_key_release(lua), on_text_enter(lua), on_mouse_move(lua),
    on_mouse_scroll(lua), on_mouse_press(lua), on_mouse_release(lua) {
  this->reload(lua);
}

auto Afk::ScriptComponent::reload(lua_State *lua) -> void {
  this->last_file_update = std::filesystem::last_write_time(this->script_path);
  if (luaL_dofile(lua, script_path.generic_string().c_str()) != 0) {
    throw std::runtime_error{"Error loading " + this->script_path.string() +
                             ": " + lua_tostring(lua, -1)};
  }
  this->on_update        = luabridge::getGlobal(lua, "update");
  this->on_key_press     = luabridge::getGlobal(lua, "keyDown");
  this->on_key_release   = luabridge::getGlobal(lua, "keyUp");
  this->on_mouse_move    = luabridge::getGlobal(lua, "mouseMove");
  this->on_mouse_press   = luabridge::getGlobal(lua, "mouseDown");
  this->on_mouse_release = luabridge::getGlobal(lua, "mouseUp");
  this->on_mouse_scroll  = luabridge::getGlobal(lua, "mouseScroll");
  this->on_text_enter    = luabridge::getGlobal(lua, "textEnter");
}

auto Afk::ScriptComponent::reload_if_old(lua_State *lua) -> void {
  if (std::filesystem::last_write_time(this->script_path) > this->last_file_update) {
    this->reload(lua);
  }
}

auto Afk::ScriptComponent::update(float dt) -> void {
  if (this->on_update.isFunction())
    this->on_update(dt);
}
auto Afk::ScriptComponent::key_press(int key, bool alt, bool ctrl, bool shift) -> void {
  if (this->on_key_press.isFunction()) {
    this->on_key_press(key, alt, ctrl, shift);
  }
}
auto Afk::ScriptComponent::key_release(int key, bool alt, bool ctrl, bool shift) -> void {
  if (this->on_key_release.isFunction()) {
    this->on_key_release(key, alt, ctrl, shift);
  }
}
auto Afk::ScriptComponent::text_enter(const std::string &text) -> void {
  if (this->on_text_enter.isFunction()) {
    this->on_text_enter(text);
  }
}
auto Afk::ScriptComponent::mouse_move(int mousex, int mousey) -> void {
  if (this->on_mouse_move.isFunction()) {
    this->on_mouse_move(mousex, mousey);
  }
}
auto Afk::ScriptComponent::mouse_scroll(float delta) -> void {
  if (this->on_mouse_scroll.isFunction()) {
    this->on_mouse_scroll(delta);
  }
}
auto Afk::ScriptComponent::mouse_press(int button) -> void {
  if (this->on_mouse_press.isFunction()) {
    this->on_mouse_press(button);
  }
}
auto Afk::ScriptComponent::mouse_release(int button) -> void {
  if (this->on_mouse_release.isFunction()) {
    this->on_mouse_release(button);
  }
}
