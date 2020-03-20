#include "ScriptComponent.hpp"

#include <stdexcept>

#include "ScriptKeyboard.hpp"
#include "ScriptMouse.hpp"

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
    : scriptFilename(filename), onUpdate(lua), onKeyPress(lua),
      onKeyRelease(lua), onTextEnter(lua), onMouseMove(lua), onMouseScroll(lua),
      onMousePress(lua), onMouseRelease(lua) {
    if (luaL_dofile(lua, scriptFilename.c_str()) != 0) {
        throw std::runtime_error{"Error loading " + filename + ": " +
                                 lua_tostring(lua, -1)};
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

auto ScriptComponent::Update(float dt) -> void {
    if (this->onUpdate.isFunction())
        this->onUpdate(dt);
}
