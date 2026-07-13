#include "sys/core/InputSystem.hpp"

#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"

#include <algorithm>

namespace mtrs::sys
{

comp::KeyButtons *InputSystem::key_buttons;
comp::MouseButtons *InputSystem::mouse_buttons;
comp::MouseScroll *InputSystem::mouse_scroll;

void InputSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    std::copy_n(key_buttons->keys, 349, key_buttons->last_keys);
    std::copy_n(mouse_buttons->buttons, 8, mouse_buttons->last_buttons);
    mouse_scroll->last_scroll = mouse_scroll->scroll;
}

}