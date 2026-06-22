#include "comp/single/MouseButtons.hpp"

#include <cstring>

namespace mtrs::comp
{

MouseButtons::MouseButtons(void*)
{
    std::memset(buttons, 0, sizeof(buttons));
}

}
