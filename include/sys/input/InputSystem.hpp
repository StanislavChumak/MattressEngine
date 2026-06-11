#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "glm/fwd.hpp"
// #include <vector>
// #include <functional>

namespace mtrs::comp {
    class ECSWorld;
    struct Input;
}

namespace mtrs::sys
{

class InputSystem
{
    comp::Input &input;
    // std::vector<std::function<void()>> _subscribersKey[349][3];
    // std::vector<std::function<void()>> _subscribersMouseButton[8][2];
    // std::vector<std::function<void(glm::dvec2)>> _subscribersCursor;
public:
    InputSystem(comp::Input &input);

    void setKey(comp::ECSWorld &world, int key, bool action);
    void setMouseButton(comp::ECSWorld &world, int button, bool action);

    // void keySubscribe(int key, bool action, std::function<void()> subscriber);
    // void mouseButtonSubscribe(int botton, bool action, std::function<void()> subscriber);
    // void cursorSubscribe(std::function<void(glm::dvec2)> subscriber);

    void updateLastInput();
};

}

#endif
