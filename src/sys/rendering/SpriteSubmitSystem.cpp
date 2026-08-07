#include "sys/rendering/SpriteSubmitSystem.hpp"

#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Render.hpp"
#include "comp/core/Transform.hpp"
#include "comp/rendering/Sprite.hpp"

namespace mtrs::sys
{

res::InstanceData sprite_to_instance(comp::Transform *transform, comp::Sprite *sprite)
{
    res::InstanceData data;
    const glm::mat4 &m = transform->matrix.get();

    data.position = glm::vec2(m[3].x, m[3].y);

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(m[0].x, m[0].y));
    scale.y = glm::length(glm::vec2(m[1].x, m[1].y));
    data.size = (glm::vec2)sprite->size * scale;

    data.rotation = std::atan2(m[0].y, m[0].x);

    data.lb_uv = sprite->sub_texture.lb_vertex;
    data.rt_uv = sprite->sub_texture.rt_vertex;
    data.color = sprite->color;

    return data;
}

comp::Render *SpriteSubmitSystem::render;

void SpriteSubmitSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, transform, sprite] : world.view<comp::Transform, comp::Sprite>())
    {
        if(!sprite->visibility) continue;

        render->submit_batch(sprite->shader, sprite->texture, sprite->layer,
            {sprite_to_instance(transform, sprite)});
    }
}

}