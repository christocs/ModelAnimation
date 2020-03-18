#include <cstdint>
#include <string>

#include <LuaBridge/LuaBridge.h>

using luabridge::LuaRef;

class LuaRef;

class ScriptComponent {
  private:
    // Engine
    LuaRef onUpdate;
    LuaRef onDraw;
    // Keyboard
    LuaRef onKeyPress;
    LuaRef onKeyRelease;
    LuaRef onTextEnter;
    // Mouse
    LuaRef onMouseMove;
    LuaRef onMouseScroll;
    LuaRef onMousePress;
    LuaRef onMouseRelease;

  public:
    // Assuming DT is float for now, will change if needed.
    auto Update(float dt) -> void;
    auto Draw() -> void;
    // TODO: Façade sf::Keyboard::Key
    auto KeyPress(int key, bool alt, bool ctrl, bool shift) -> void;
    auto KeyRelease(int key, bool alt, bool ctrl, bool shift) -> void;
    auto TextEnter(uint32_t text) -> void;
    auto MouseMove(int mousex, int mousey) -> void;
    // No point in supporting multiple mouse wheels (although SFML does)
    auto MouseScroll(float delta, int mousex, int mousey) -> void;
    // TODO: Façade sf::Mouse::Button
    auto MousePress(int btn, int mousex, int mousey) -> void;
    auto MouseRelease(int btn, int mousex, int mousey) -> void;
}