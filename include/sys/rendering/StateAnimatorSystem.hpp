#ifndef STATE_ANIMATOR_SYSTEM_HPP
#define STATE_ANIMATOR_SYSTEM_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class StateAnimatorSystem
{
public:
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
