#include "sys/transform/GlobalTransformSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/core/ParentChildren.hpp"
#include "comp/core/Transform.hpp"

void update_tree(mtrs::comp::ECSWorld &world, EntityID id, glm::mat4 parentMatrix)
{
    mtrs::comp::Transform *transform = world.get_comp<mtrs::comp::Transform>(id);
    mtrs::comp::Children *children = world.get_comp<mtrs::comp::Children>(id);

    if(transform)
    {
        transform->update_local_matrix();
        transform->global_matrix = parentMatrix * transform->local_matrix;
        parentMatrix = transform->global_matrix;
    }
    if(children)
    {
        for(EntityID child : children->childrensIds)
            update_tree(world, child, parentMatrix);
    }
}

namespace mtrs::sys
{

void GlobalTransformSystem::update(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, transform] : world.view<comp::Transform>())
    {
        if (!transform->dirty)
            continue;

        EntityID rootId = entity;
        comp::Parent *parent = world.get_comp<comp::Parent>(entity);
        while (parent)
        {
            rootId = parent->parentId;
            parent = world.get_comp<comp::Parent>(rootId);
        }

        update_tree(world, rootId, glm::mat4(1.0f));
    }
}

}