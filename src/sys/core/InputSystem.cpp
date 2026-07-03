#include "sys/core/InputSystem.hpp"

#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"

#include <algorithm>

namespace mtrs::sys
{

void InputSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    std::copy_n(key_buttons->keys, 349, key_buttons->last_keys);
    std::copy_n(mouse_buttons->buttons, 8, mouse_buttons->last_buttons);
}

}