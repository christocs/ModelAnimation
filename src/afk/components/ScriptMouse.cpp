#include "ScriptMouse.hpp"
auto LuaMouse::getButtons() -> std::vector<Mouse> & {
    static std::vector<Mouse> s{
        {"Left", sf::Mouse::Button::Left},
        {"Right", sf::Mouse::Button::Right},
        {"Middle", sf::Mouse::Button::Middle},
        {"Extra1", sf::Mouse::Button::XButton1},
        {"Extra2", sf::Mouse::Button::XButton2},
    };
    return s;
}
