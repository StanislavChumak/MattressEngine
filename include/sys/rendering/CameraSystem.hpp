#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class CameraSystem
{
public:
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
