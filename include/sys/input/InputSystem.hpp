#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::comp {
    class ECSWorld;
    struct KeyButtons;
    struct MouseButtons;
}

namespace mtrs::sys
{

class InputSystem : public System<InputSystem>
{
public:
    inline static comp::KeyButtons *key_buttons;
    inline static comp::MouseButtons *mouse_buttons;

    void update_imp(comp::ECSWorld &world, const double &delta);
    constexpr int get_prioritet_imp() { return SystemPriority::INPUT; }
};

}

#endif
