#include "ScriptComponent.hpp"

#include <stdexcept>

#include "ScriptKeyboard.hpp"
#include "ScriptMouse.hpp"
#include "afk/io/Path.hpp"

//
// /**
//  * This should probably be moved somewhere better
//  */
// auto Afk::ScriptComponent::setup_lua_state(lua_State *lua) -> void {
//   auto keyns = luabridge::getGlobalNamespace(lua).beginNamespace("key");
//   for (auto &key : LuaKeyboard::getKeys()) {
//     // key.code can't be changed from lua's side
//     keyns.addVariable<int>(key.name.c_str(), &key.code, false);
//   }
//   keyns.endNamespace();
//   auto mousens = luabridge::getGlobalNamespace(lua).beginNamespace("mouse");
//   for (auto &btn : LuaMouse::getButtons()) {
//     mousens.addVariable<int>(btn.name.c_str(), &btn.button, false);
//   }
//   mousens.endNamespace();
// }
//
// Afk::ScriptComponent::ScriptComponent(lua_State *lua, const std::string &filename)
//   : scriptPath(Afk::Path::getAbsolutePath("script/" + filename)), on_update(lua),
//     on_key_press(lua), on_key_release(lua), on_text_enter(lua), on_mouse_move(lua),
//     on_mouse_scroll(lua), on_mouse_press(lua), on_mouse_release(lua) {
//   this->reload(lua);
// }
//
// auto Afk::ScriptComponent::reload(lua_State *lua) -> void {
//   this->lastFileUpdate = std::filesystem::last_write_time(this->scriptPath);
//   if (luaL_dofile(lua, scriptPath.generic_string().c_str()) != 0) {
//     throw std::runtime_error{"Error loading " + this->scriptPath.string() +
//                              ": " + lua_tostring(lua, -1)};
//   }
//   this->on_update       = luabridge::getGlobal(lua, "update");
//   this->on_key_press     = luabridge::getGlobal(lua, "keyDown");
//   this->on_key_release   = luabridge::getGlobal(lua, "keyUp");
//   this->on_mouse_move    = luabridge::getGlobal(lua, "mouseMove");
//   this->on_mouse_press   = luabridge::getGlobal(lua, "mouseDown");
//   this->on_mouse_release = luabridge::getGlobal(lua, "mouseUp");
//   this->on_mouse_scroll  = luabridge::getGlobal(lua, "mouseScroll");
//   this->on_text_enter    = luabridge::getGlobal(lua, "textEnter");
// }
//
// auto Afk::ScriptComponent::reloadIfOld(lua_State *lua) -> void {
//   if (std::filesystem::last_write_time(this->scriptPath) > this->lastFileUpdate) {
//     this->reload(lua);
//   }
// }
//
// auto Afk::ScriptComponent::update(float dt) -> void {
//   if (this->on_update.isFunction())
//     this->on_update(dt);
// }
// auto Afk::ScriptComponent::keyPress(sf::Keyboard::Key key, bool alt, bool ctrl,
//                                     bool shift) -> void {
//   if (this->on_key_press.isFunction()) {
//     this->on_key_press(static_cast<int>(key), alt, ctrl, shift);
//   }
// }
// auto Afk::ScriptComponent::keyRelease(sf::Keyboard::Key key, bool alt,
//                                       bool ctrl, bool shift) -> void {
//   if (this->on_key_release.isFunction()) {
//     this->on_key_release(static_cast<int>(key), alt, ctrl, shift);
//   }
// }
// auto Afk::ScriptComponent::textEnter(const std::string &text) -> void {
//   if (this->on_text_enter.isFunction()) {
//     this->on_text_enter(text);
//   }
// }
// auto Afk::ScriptComponent::mouseMove(int mousex, int mousey) -> void {
//   if (this->on_mouse_move.isFunction()) {
//     this->on_mouse_move(mousex, mousey);
//   }
// }
// auto Afk::ScriptComponent::mouseScroll(float delta, int mousex, int mousey) -> void {
//   if (this->on_mouse_scroll.isFunction()) {
//     this->on_mouse_scroll(delta, mousex, mousey);
//   }
// }
// auto Afk::ScriptComponent::mousePress(sf::Mouse::Button button, int mousex, int mousey)
//     -> void {
//   if (this->on_mouse_press.isFunction()) {
//     this->on_mouse_press(static_cast<int>(button), mousex, mousey);
//   }
// }
// auto Afk::ScriptComponent::mouseRelease(sf::Mouse::Button button, int mousex, int mousey)
//     -> void {
//   if (this->on_mouse_release.isFunction()) {
//     this->on_mouse_release(static_cast<int>(button), mousex, mousey);
//   }
// }
