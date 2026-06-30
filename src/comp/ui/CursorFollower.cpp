#include "comp/ui/CursorFollower.hpp"

#include <fstream>

#include "comp_struct/CursorFollower.struct"

namespace mtrs::comp
{

CursorFollower::CursorFollower(COMPONENT_ARGS)
{
    CursorFollower_sc follower;
    file.read(reinterpret_cast<char*>(&follower), sizeof(follower));

    offset.x = follower.offset_pos_x;
    offset.y = follower.offset_pos_y;
}

}