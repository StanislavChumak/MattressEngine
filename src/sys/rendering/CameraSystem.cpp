#include "sys/rendering/CameraSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Camera.hpp"
#include "comp/core/Transform.hpp"

namespace mtrs::sys
{

void CameraSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    // comp::Camera *camera = world.component_manager().get_single_comp<comp::Camera>();
    // if(!camera || camera->target == NULL_ENTITY) return;
    // comp::Transform *transform = world.component_manager().get_comp<comp::Transform>(camera->target);
    // if(!transform) return;
 
    // camera->update_view_matrix(glm::inverse(transform->global_matrix));
}

}