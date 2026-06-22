#include "comp/single/KeyButtons.hpp"

#include <cstring>

namespace mtrs::comp
{

KeyButtons::KeyButtons(void*)
{
    std::memset(keys, 0, sizeof(keys));
}

}
