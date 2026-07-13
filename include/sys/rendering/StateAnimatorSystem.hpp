#ifndef STATE_ANIMATOR_SYSTEM_HPP
#define STATE_ANIMATOR_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class StateAnimatorSystem : public System<StateAnimatorSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::ANIMATION; }
    static constexpr const char *get_system_name_imp() { return "StateAnimatorSystem"; }
};

}

#endif
