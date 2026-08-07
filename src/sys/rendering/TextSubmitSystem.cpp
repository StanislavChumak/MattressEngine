#include "sys/rendering/TextSubmitSystem.hpp"

#include "res/asset/Font.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Render.hpp"
#include "comp/single/GlyphDecoder.hpp"
#include "comp/core/Transform.hpp"
#include "comp/ui/Label.hpp"

namespace mtrs::sys
{

std::vector<res::InstanceData> label_to_instances(const glm::mat4 &m, glm::vec2 &current_pos,
    std::vector<mtrs::res::Glyph> glyphs, glm::highp_u8vec4 color)
{
    std::vector<res::InstanceData> data;
    res::InstanceData instance;

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(m[0].x, m[0].y));
    scale.y = glm::length(glm::vec2(m[1].x, m[1].y));
    
    instance.rotation = std::atan2(m[0].y, m[0].x);
    instance.color = color;
    
    data.assign(glyphs.size(), std::move(instance));
    
    auto iter = data.begin();
    for(auto &glyph : glyphs)
    {
        iter->size = glyph.size * scale;
        iter->position = current_pos;
        iter->position += iter->size / 2.f;
        current_pos.x += glyph.size.x + 1;
        iter->lb_uv = glyph.sub_texture.lb_vertex;
        iter->rt_uv = glyph.sub_texture.rt_vertex;
        iter++;
    }

    return data;
}

void put_space(const glm::mat4 &m, glm::vec2 &current_pos,
    std::vector<mtrs::res::Glyph> glyphs, const glm::uvec2 &space_size)
{
    for(auto &glyph : glyphs)
    {
        switch (glyph.symbol)
        {
        case ' ':
            current_pos.x += space_size.x;
            break;
        case '\n':
            current_pos.x = m[3].x;
            current_pos.y -= space_size.y + 1;
            break;
        case '\t':
            current_pos.x += space_size.x * 4;
            break;
        default:
            break;
        }
    }
}

comp::Render *TextSubmitSystem::render;
comp::GlyphDecoder *TextSubmitSystem::decoder;

void TextSubmitSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, transform, label] : world.view<comp::Transform, comp::Label>())
    {
        if(!label->visibility) continue;

        const glm::mat4 &m = transform->matrix.get();

        glm::vec2 current_pos(m[3].x,
            m[3].y - label->text.space_size.y);

        for(auto sub_text : label->text.glyphs)
        {
            if(!label->text.fonts[sub_text.first])
            {
                put_space(m, current_pos, sub_text.second, label->text.space_size);
            }
            else
            {
                render->submit_batch(label->shader, label->text.fonts[sub_text.first]->texture,
                    label->layer, label_to_instances(m, current_pos, sub_text.second, label->color));
            }
        }
    }
}

}