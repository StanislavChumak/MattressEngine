#ifndef INPUT_SYBSCRUBERS_SYSTEM_HPP
#define INPUT_SYBSCRUBERS_SYSTEM_HPP

#include "sys/System.hpp"
#include <vector>

namespace mtrs::comp
{
    struct KeyButtons;
    struct MouseButtons;
    struct MouseScroll;
}

namespace mtrs::sys
{

class InputSybscrubersSystem : public System<InputSybscrubersSystem>
{
public:
    static std::vector<void(*)()> *callbacks;

    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::GAMEPLAY_EARLY; }
    static constexpr const char *get_system_name_imp() { return "InputSybscrubersSystem"; }
};

}

#endif
