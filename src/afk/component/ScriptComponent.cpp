#include "afk/component/ScriptComponent.hpp"

#include <filesystem>
#include <stdexcept>
#include <string>

#include "afk/event/EventManager.hpp"
#include "afk/io/Path.hpp"
#include "afk/script/Script.hpp"

using namespace std::string_literals;
using std::runtime_error;
using std::filesystem::path;

/**
 * This should probably be moved somewhere better
 */
auto Afk::ScriptComponent::setup_lua_state(lua_State *lua) -> void {
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

  auto event_manager_class =
      luabridge::getGlobalNamespace(lua).beginClass<EventManager>("events");
  for (const auto &event : Afk::Script::events) {
    event_manager_class.addStaticProperty<int>(event.name.c_str(),
                                               const_cast<int *>(&event.type), false);
  }

  // TODO: Think of a way to allow live reloading with this registration type
  // (Right now when you reload the file I think old registered events will stay around)
  // (Maybe need some alternate register function that lua will use)
  // evtmgr.addFunction("register", &Afk::EventManager::register_event);
  event_manager_class.endClass();
}

Afk::ScriptComponent::ScriptComponent(lua_State *lua, path file_name)
  : file_path("script" / file_name), on_update(lua), on_key_press(lua),
    on_key_release(lua), on_text_enter(lua), on_mouse_move(lua),
    on_mouse_scroll(lua), on_mouse_press(lua), on_mouse_release(lua) {
  this->reload(lua);
}

auto Afk::ScriptComponent::reload(lua_State *lua) -> void {
  const auto abs_path = Afk::get_resource_path(this->file_path);

  this->last_file_update = std::filesystem::last_write_time(abs_path);

  if (luaL_dofile(lua, abs_path.string().c_str()) != 0) {
    throw runtime_error{"Error loading "s + this->file_path.string() + ": "s +
                        lua_tostring(lua, -1)};
  }

  this->on_update        = luabridge::getGlobal(lua, "update");
  this->on_key_press     = luabridge::getGlobal(lua, "key_down");
  this->on_key_release   = luabridge::getGlobal(lua, "key_up");
  this->on_mouse_move    = luabridge::getGlobal(lua, "mouse_move");
  this->on_mouse_press   = luabridge::getGlobal(lua, "mouse_down");
  this->on_mouse_release = luabridge::getGlobal(lua, "mouse_up");
  this->on_mouse_scroll  = luabridge::getGlobal(lua, "mouse_scroll");
  this->on_text_enter    = luabridge::getGlobal(lua, "text_enter");
}

auto Afk::ScriptComponent::reload_if_old(lua_State *lua) -> void {
  const auto abs_path = Afk::get_resource_path(this->file_path);

  if (std::filesystem::last_write_time(abs_path) > this->last_file_update) {
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

auto Afk::ScriptComponent::mouse_move(int x, int y) -> void {
  if (this->on_mouse_move.isFunction()) {
    this->on_mouse_move(x, y);
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
