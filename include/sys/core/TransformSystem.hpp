#ifndef TRANSFORM_SYSTEM_HPP
#define TRANSFORM_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class TransformSystem : public System<TransformSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::TRANSFORM; }
    static constexpr const char *get_system_name_imp() { return "TransformSystem"; }
};

}

#endif
