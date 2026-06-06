#ifndef ANIMATOR_SYSTEM_HPP
#define ANIMATOR_SYSTEM_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class AnimatorSystem
{
public:
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
