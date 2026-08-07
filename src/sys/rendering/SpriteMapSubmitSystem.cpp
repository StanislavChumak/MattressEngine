#include "sys/rendering/SpriteMapSubmitSystem.hpp"

#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Render.hpp"
#include "comp/core/Transform.hpp"
#include "comp/rendering/SpriteMap.hpp"

namespace mtrs::sys
{

std::vector<res::InstanceData> map_to_instances(comp::Transform *transform, comp::SpriteMap *map)
{
    std::vector<res::InstanceData> data;

    res::InstanceData instance;

    const glm::mat4 &m = transform->matrix.get();

    instance.position = glm::vec2(m[3].x, m[3].y);

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(m[0].x, m[0].y));
    scale.y = glm::length(glm::vec2(m[1].x, m[1].y));
    instance.size = (glm::vec2)map->cell_size * scale;

    instance.rotation = std::atan2(m[0].y, m[0].x);
    instance.color = map->color;

    data.assign(map->cell_map.size(), instance);

    auto iter = data.begin();
    for(auto &cell : map->cell_map)
    {
        iter->position += (glm::vec2)cell.cord * instance.size;
        iter->lb_uv = map->cell_types[cell.type].lb_vertex;
        iter->rt_uv = map->cell_types[cell.type].rt_vertex;
        iter++;
    }

    return data;
}

comp::Render *SpriteMapSubmitSystem::render;

void SpriteMapSubmitSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, transform, map] : world.view<comp::Transform, comp::SpriteMap>())
    {
        if(!map->visibility) continue;

        render->submit_batch(map->shader, map->texture, map->layer,
            map_to_instances(transform, map));
    }
}

}