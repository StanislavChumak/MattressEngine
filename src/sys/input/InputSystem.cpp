#include "sys/input/InputSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Input.hpp"
#include "comp/single/Cursor.hpp"

namespace mtrs::sys
{

InputSystem::InputSystem(comp::Input &input):input(input){}

void InputSystem::setKey(comp::ECSWorld &world, int key, bool action)
{
    input.keys[key] = action;
    // for(auto func : _subscribersKey[key][action])
    // {
    //     func();
    // }
}

void InputSystem::setMouseButton(comp::ECSWorld &world, int button, bool action)
{
    input.mouse_buttons[button] = action;
    // for(auto func : _subscribersMouseButton[button][action])
    // {
    //     func();
    // }
}

void InputSystem::setCursor(comp::ECSWorld &world, glm::dvec2 pos)
{
    comp::Cursor *cursor = world.get_single_comp<comp::Cursor>();
    if(!cursor) return;
    cursor->pos = pos;
    // for(auto func : _subscribersCursor)
    // {
    //     func(cursor);
    // }
}

// void InputSystem::keySubscribe(int key, bool action, std::function<void()> subscriber)
// {
//     _subscribersKey[key][action].push_back(subscriber);
// }

// void InputSystem::mouseButtonSubscribe(int botton, bool action, std::function<void()> subscriber)
// {
//     _subscribersMouseButton[botton][action].push_back(subscriber);
// }

// void InputSystem::cursorSubscribe(std::function<void(glm::dvec2)> subscriber)
// {
//     _subscribersCursor.push_back(subscriber);
// }

void InputSystem::updateLastInput()
{
    for(int i = 0; i < 349; i++)
        input.last_keys[i] = input.keys[i];
    for(int i = 0; i < 8; i++)
        input.last_mouse_buttons[i] = input.mouse_buttons[i];
}

}