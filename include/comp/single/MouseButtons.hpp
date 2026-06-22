#ifndef MOUSE_BUTTONS_HPP
#define MOUSE_BUTTONS_HPP

namespace mtrs::comp
{

struct MouseButtons
{
    bool buttons[8];
    bool last_buttons[8];

    MouseButtons() = delete;
    ~MouseButtons() = default;
    MouseButtons(const MouseButtons&) = delete;
    MouseButtons &operator=(const MouseButtons&) = delete;
    MouseButtons(MouseButtons&&) = delete;
    MouseButtons &operator=(MouseButtons&&) = delete;
    
    MouseButtons(void*);
};


}

#endif
