#ifndef GLOBAL_TRANSFORM_SYSTEM_HPP
#define GLOBAL_TRANSFORM_SYSTEM_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class GlobalTransformSystem
{
public:
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
