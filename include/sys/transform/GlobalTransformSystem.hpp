#ifndef GLOBAL_TRANSFORM_SYSTEM_HPP
#define GLOBAL_TRANSFORM_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class GlobalTransformSystem : public System<GlobalTransformSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::TRANSFORM; };
};

}

#endif
