#include "sys/rendering/SpriteRenderSystem.hpp"

#include "res/asset/RenderContext.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/core/Transform.hpp"
#include "comp/rendering/Sprite.hpp"

mtrs::res::InstanceData sprite_to_instance(mtrs::comp::Transform *transform, mtrs::comp::Sprite *sprite)
{
    mtrs::res::InstanceData date;
    const glm::mat4 &m = transform->matrix.get();

    date.position = glm::vec2(m[3].x, m[3].y);

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(m[0].x, m[0].y));
    scale.y = glm::length(glm::vec2(m[1].x, m[1].y));
    date.size = (glm::vec2)sprite->size * scale;

    date.rotation = std::atan2(m[0].y, m[0].x);

    date.lb_uv = sprite->sub_texture.lb_vertex;
    date.rt_uv = sprite->sub_texture.rt_vertex;
    date.color = sprite->color;
    date.layer = sprite->layer;

    return date;
}

namespace mtrs::sys
{

std::shared_ptr<res::RenderContext> SpriteRenderSystem::context;

void SpriteRenderSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    context->begin_batches();

    for(auto [entity, transform, sprite] : world.view<comp::Transform, comp::Sprite>())
    {
        if(!sprite->visibility) continue;

        u_int64_t id = sprite->shader->id() | u_int64_t(sprite->texture->id()) << 32;
        context->submit_batch(id, sprite_to_instance(transform, sprite));
    }

    context->end_batches();
}

}