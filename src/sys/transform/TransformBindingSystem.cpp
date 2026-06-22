#include "sys/transform/TransformBindingSystem.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/core/ParentChildren.hpp"
#include "comp/core/Transform.hpp"

namespace mtrs::sys
{

void TransformBindingSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, transform, parent] : world.view<comp::Transform, comp::Parent>())
    {
        // comp::Transform *parent_transform = world.component_manager().get_comp<comp::Transform>(parent->parentId);

        // if(!parent_transform || transform->matrix.valid_dep<3>()) continue;

        // transform->matrix.set_calculator(
        //     [](const glm::vec2 *pos, const glm::vec2 *sc_size, const float *rot, const glm::mat4 *prt_mtx)
        //     {
        //         glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(*pos, 0.0f));
        //         m = glm::rotate(m, glm::radians(*rot), glm::vec3(0.0f, 0.0f, 1.0f));
        //         m = glm::scale(m, glm::vec3(*sc_size, 1.0f));
        //         m = *prt_mtx * m;
        //         return m;
        //     });
        // transform->matrix.set_dep<3>(&parent_transform->matrix);
    }
}

}