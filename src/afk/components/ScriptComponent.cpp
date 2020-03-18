#include "ScriptComponent.hpp"

#include <stdexcept>

ScriptComponent::ScriptComponent(lua_State *lua, const std::string &filename)
    : scriptFilename(filename), onUpdate(lua), onDraw(lua), onKeyPress(lua),
      onKeyRelease(lua), onTextEnter(lua), onMouseMove(lua), onMouseScroll(lua),
      onMousePress(lua), onMouseRelease(lua) {
    if (luaL_dofile(lua, scriptFilename.c_str()) != 0) {
        throw std::runtime_error{"Error loading " + filename + ": " +
                                 lua_tostring(lua, -1)};
    }
    this->onUpdate       = luabridge::getGlobal(lua, "update");
    this->onDraw         = luabridge::getGlobal(lua, "draw");
    this->onKeyPress     = luabridge::getGlobal(lua, "keyDown");
    this->onKeyRelease   = luabridge::getGlobal(lua, "keyUp");
    this->onMouseMove    = luabridge::getGlobal(lua, "mouseMove");
    this->onMousePress   = luabridge::getGlobal(lua, "mouseDown");
    this->onMouseRelease = luabridge::getGlobal(lua, "mouseUp");
    this->onMouseScroll  = luabridge::getGlobal(lua, "mouseScroll");
    this->onTextEnter    = luabridge::getGlobal(lua, "textEnter");
}
