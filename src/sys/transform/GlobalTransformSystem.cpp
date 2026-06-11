#include "sys/transform/GlobalTransformSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/core/Transform.hpp"


namespace mtrs::sys
{

void GlobalTransformSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, transform] : world.view<comp::Transform>())
    {
        transform->matrix.update();
    }
}

}