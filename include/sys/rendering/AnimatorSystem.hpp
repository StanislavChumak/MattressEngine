#ifndef ANIMATOR_SYSTEM_HPP
#define ANIMATOR_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class AnimatorSystem : public System<AnimatorSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::ANIMATION; };
};

}

#endif
