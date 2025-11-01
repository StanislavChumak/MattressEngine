#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "../../ECSWorld.h"
#include "../../include/components/core/Input.h"

#include <vector>
#include <functional>

class InputSystem
{
    std::vector<std::function<void()>> _subscribersKey[349][3];
    std::vector<std::function<void()>> _subscribersMouseButton[8][2];
    std::vector<std::function<void(glm::uvec2)>> _subscribersCursor;
public:
    void setKey(ECSWorld &component, int key, Input::Action action);
    void setMouseButton(ECSWorld &component, int button, Input::Action action);
    void setCursor(ECSWorld &component, glm::uvec2 cursor);

    void keySubscribe(int key, Input::Action action, std::function<void()> subscriber);
    void mouseButtonSubscribe(int botton, Input::Action action, std::function<void()> subscriber);
    void cursorSubscribe(std::function<void(glm::uvec2)> subscriber);
};

#endif
