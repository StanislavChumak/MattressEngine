#ifndef CURSOR_FOLLOWERS_SYSTEM_HPP
#define CURSOR_FOLLOWERS_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class CursorFollowersSystem : public System<CursorFollowersSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::TRANSFORM; };
};

}

#endif
