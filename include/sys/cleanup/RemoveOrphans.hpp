#ifndef REMOVE_ORPHANS_HPP
#define REMOVE_ORPHANS_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class RemoveOrphans : public System<RemoveOrphans>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::CLEANUP; };
};

}

#endif
