#include "sys/rendering/SpriteMapRenderSystem.hpp"

#include "res/asset/RenderContext.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/core/Transform.hpp"
#include "comp/rendering/SpriteMap.hpp"

mtrs::res::InstanceData map_cell_to_instance(mtrs::comp::Transform *transform, mtrs::comp::SpriteMap *map, mtrs::comp::SpriteMap::MapCell &cell)
{
    mtrs::res::InstanceData data;
    const glm::mat4 &m = transform->matrix.get();

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(m[0].x, m[0].y));
    scale.y = glm::length(glm::vec2(m[1].x, m[1].y));
    data.size = (glm::vec2)map->cell_size * scale;

    data.position = glm::vec2(m[3].x, m[3].y) + ((glm::vec2)cell.cord + glm::vec2(0.5f)) * data.size;

    data.rotation = std::atan2(m[0].y, m[0].x);

    data.lb_uv = map->cell_types[cell.type].lb_vertex;
    data.rt_uv = map->cell_types[cell.type].rt_vertex;
    data.color = map->color;
    data.layer = map->layer;

    return data;
}

namespace mtrs::sys
{

std::shared_ptr<res::RenderContext> SpriteMapRenderSystem::context;

void SpriteMapRenderSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    context->begin_batches();

    for(auto [entity, transform, map] : world.view<comp::Transform, comp::SpriteMap>())
    {
        if(!map->visibility) continue;

        uint64_t id = map->shader->id() | uint64_t(map->texture->id()) << 32;
        for(auto &cell : map->cell_map)
        {
            context->submit_batch(id, map_cell_to_instance(transform, map, cell));
        }
    }

    context->end_batches();
}

}