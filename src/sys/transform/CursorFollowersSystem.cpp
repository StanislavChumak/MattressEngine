#include "sys/transform/CursorFollowersSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Cursor.hpp"
#include "comp/ui/CursorFollower.hpp"
#include "comp/core/Transform.hpp"

namespace mtrs::sys
{

void CursorFollowersSystem::update(comp::ECSWorld &world, const double &delta)
{
    comp::Cursor *cursor = world.get_single_comp<comp::Cursor>();
    if(!cursor) return;
    for(auto [entity, transform, follower] : world.view<comp::Transform, comp::CursorFollower>())
    {
        transform->set_position(cursor->pos + follower->offset);
    }
}

}