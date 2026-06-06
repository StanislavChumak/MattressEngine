#ifndef CURSOR_FOLLOWERS_SYSTEM_HPP
#define CURSOR_FOLLOWERS_SYSTEM_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class CursorFollowersSystem
{
public:
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
