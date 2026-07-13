#ifndef MAP_ANIMATOR_SYSTEM_HPP
#define MAP_ANIMATOR_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class MapAnimatorSystem : public System<MapAnimatorSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::ANIMATION; }
    static constexpr const char *get_system_name_imp() { return "MapAnimatorSystem"; }
};

}

#endif
