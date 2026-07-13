#include "sys/rendering/CameraSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Camera.hpp"

namespace mtrs::sys
{

    comp::Camera *CameraSystem::camera;

void CameraSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    if(camera->target_matrix)
    {
        camera->update_view_matrix();
    }
}

}