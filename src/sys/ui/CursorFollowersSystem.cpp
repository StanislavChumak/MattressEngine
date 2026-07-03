#include "sys/ui/CursorFollowersSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Cursor.hpp"
#include "comp/ui/CursorFollower.hpp"
#include "comp/core/Transform.hpp"

namespace mtrs::sys
{

void CursorFollowersSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    comp::Cursor *cursor = world.single_comp<comp::Cursor>();
    if(!cursor) return;
    for(auto [entity, transform, follower] : world.view<comp::Transform, comp::CursorFollower>())
    {
        transform->position.set(cursor->position.get() + follower->offset);
    }
}

}