#ifndef SCRIPTS_SYSTEM_HPP
#define SCRIPTS_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class ScriptsSystem : public System<ScriptsSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::GAMEPLAY_EARLY; }
};

}

#endif
