#include "ScriptComponent.hpp"

#include <stdexcept>

#include "ScriptKeyboard.hpp"
#include "ScriptMouse.hpp"
#include "afk/io/Path.hpp"

/**
 * This should probably be moved somewhere better
 */
auto ScriptComponent::SetupLuaState(lua_State *lua) -> void {
  auto keyns = luabridge::getGlobalNamespace(lua).beginNamespace("key");
  for (auto &key : LuaKeyboard::getKeys()) {
    // key.code can't be changed from lua's side
    keyns.addVariable<int>(key.name.c_str(), &key.code, false);
  }
  keyns.endNamespace();
  auto mousens = luabridge::getGlobalNamespace(lua).beginNamespace("button");
  for (auto &btn : LuaMouse::getButtons()) {
    mousens.addVariable<int>(btn.name.c_str(), &btn.button, false);
  }
  mousens.endNamespace();
}

ScriptComponent::ScriptComponent(lua_State *lua, const std::string &filename)
  : scriptPath(Afk::Path::getAbsolutePath("script/" + filename)), onUpdate(lua),
    onKeyPress(lua), onKeyRelease(lua), onTextEnter(lua), onMouseMove(lua),
    onMouseScroll(lua), onMousePress(lua), onMouseRelease(lua) {
  this->Reload(lua);
}

auto ScriptComponent::Reload(lua_State *lua) -> void {
  this->lastFileUpdate = std::filesystem::last_write_time(this->scriptPath);
  if (luaL_dofile(lua, scriptPath.c_str()) != 0) {
    throw std::runtime_error{"Error loading " + this->scriptPath.string() +
                             ": " + lua_tostring(lua, -1)};
  }
  this->onUpdate       = luabridge::getGlobal(lua, "update");
  this->onKeyPress     = luabridge::getGlobal(lua, "keyDown");
  this->onKeyRelease   = luabridge::getGlobal(lua, "keyUp");
  this->onMouseMove    = luabridge::getGlobal(lua, "mouseMove");
  this->onMousePress   = luabridge::getGlobal(lua, "mouseDown");
  this->onMouseRelease = luabridge::getGlobal(lua, "mouseUp");
  this->onMouseScroll  = luabridge::getGlobal(lua, "mouseScroll");
  this->onTextEnter    = luabridge::getGlobal(lua, "textEnter");
}

auto ScriptComponent::ReloadIfOld(lua_State *lua) -> void {
  if (std::filesystem::last_write_time(this->scriptPath) > this->lastFileUpdate) {
    this->Reload(lua);
  }
}

auto ScriptComponent::Update(float dt) -> void {
  if (this->onUpdate.isFunction())
    this->onUpdate(dt);
}
auto ScriptComponent::KeyPress(sf::Keyboard::Key key, bool alt, bool ctrl, bool shift)
    -> void {
  if (this->onKeyPress.isFunction()) {
    this->onKeyPress(static_cast<int>(key), alt, ctrl, shift);
  }
}
auto ScriptComponent::KeyRelease(sf::Keyboard::Key key, bool alt, bool ctrl, bool shift)
    -> void {
  if (this->onKeyRelease.isFunction()) {
    this->onKeyRelease(static_cast<int>(key), alt, ctrl, shift);
  }
}
auto ScriptComponent::TextEnter(uint32_t text) -> void {
  if (this->onTextEnter.isFunction()) {
    this->onTextEnter(text);
  }
}
auto ScriptComponent::MouseMove(int mousex, int mousey) -> void {
  if (this->onMouseMove.isFunction()) {
    this->onMouseMove(mousex, mousey);
  }
}
auto ScriptComponent::MouseScroll(float delta, int mousex, int mousey) -> void {
  if (this->onMouseScroll.isFunction()) {
    this->onMouseScroll(delta, mousex, mousey);
  }
}
auto ScriptComponent::MousePress(sf::Mouse::Button button, int mousex, int mousey) -> void {
  if (this->onMousePress.isFunction()) {
    this->onMousePress(static_cast<int>(button), mousex, mousey);
  }
}
auto ScriptComponent::MouseRelease(sf::Mouse::Button button, int mousex, int mousey) -> void {
  if (this->onMouseRelease.isFunction()) {
    this->onMouseRelease(static_cast<int>(button), mousex, mousey);
  }
}
