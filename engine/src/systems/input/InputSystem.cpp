#include "InputSystem.h"


void InputSystem::setKey(ECSWorld &world, int key, Input::Action action)
{
    world.getSingleComponent<Input>().keys[key] = action;
    for(auto func : _subscribersKey[key][action])
    {
        func();
    }
}

void InputSystem::setMouseButton(ECSWorld &world, int button, Input::Action action)
{
    world.getSingleComponent<Input>().mouseButtons[button] = action;
    for(auto func : _subscribersMouseButton[button][action])
    {
        func();
    }
}

void InputSystem::setCursor(ECSWorld &world, glm::uvec2 cursor)
{
    world.getSingleComponent<Input>().cursor = cursor;
    //for(auto func : _subscribersCursor)
    {
        //func(cursor);
    }
}

void InputSystem::keySubscribe(int key, Input::Action action, std::function<void()> subscriber)
{
    _subscribersKey[key][action].push_back(subscriber);
}

void InputSystem::mouseButtonSubscribe(int botton, Input::Action action, std::function<void()> subscriber)
{
    _subscribersMouseButton[botton][action].push_back(subscriber);
}

void InputSystem::cursorSubscribe(std::function<void(glm::uvec2)> subscriber)
{
    _subscribersCursor.push_back(subscriber);
}