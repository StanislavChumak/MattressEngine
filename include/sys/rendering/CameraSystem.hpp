#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::comp
{
    struct Camera;
}

namespace mtrs::sys
{

class CameraSystem : public System<CameraSystem>
{
public:
    static comp::Camera *camera;

    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::UI_LOGIC; }
    static constexpr const char *get_system_name_imp() { return "CameraSystem"; }
};

}

#endif
