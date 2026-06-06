#ifndef INPUT_HPP
#define INPUT_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/vec2.hpp"

namespace mtrs::comp
{

struct  Input {
    bool keys[349];
    bool mouse_buttons[8];
    bool last_keys[349];
    bool last_mouse_buttons[8];

    Input()
    {
        for(int i = 0; i < 349; i++)
        {
            keys[i] = false;
            last_keys[i] = false;
        }
        for(int i = 0; i < 8; i++)
        {
            mouse_buttons[i] = false;
            last_mouse_buttons[i] = false;
        }
    }
};

}

#endif
