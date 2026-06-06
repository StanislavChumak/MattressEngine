#include "comp/rendering/Sprite.hpp"

#include "res/ResourceManager.hpp"
#include "res/render/RenderContext.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "util/get_from_file_mtrs.hpp"

#include <fstream>
#include <cstring>

#include "mtrsstruct/dynamic_field.def"
#include "mtrsstruct/comp_struct/Sprite.struct"

namespace mtrs::comp
{

Sprite::Sprite(COMPONENT_ARGS)
{
    Sprite_sc sprite;
    file.read(reinterpret_cast<char*>(&sprite), sizeof(sprite));

    std::string path_shader = util::get_string_from_mtrs_file(file, DYNAMIC_ARGS(sprite, shader));
    std::string path_texture = util::get_string_from_mtrs_file(file, DYNAMIC_ARGS(sprite, texture));
    std::string path_atlas = util::get_string_from_mtrs_file(file, DYNAMIC_ARGS(sprite, atlas));

    shader = resource.get_resource<res::ShaderProgram>(path_shader);
    texture = resource.get_resource<res::Texture>(path_texture);

    if(path_atlas != "")
    {
        atlas = resource.get_resource<res::TextureAtlas>(path_atlas);
        sub_texture = atlas->get_sub_texture(0);
    }
    else
    {
        atlas = nullptr; 
        sub_texture = res::TextureAtlas::SubTexture2D{glm::vec2(0.001f), glm::vec2(0.999f)};
    }

    std::shared_ptr<res::RenderContext> context = resource.get_cache<res::RenderContext>()["context"].lock();
    context->create_sprite_batch(shader, texture);

    layer = sprite.layer;

    size.x = sprite.size_x;
    size.y = sprite.size_y;

    uint8_t bufferColor[4];
    std::memcpy(bufferColor, &sprite.color, 4);
    
    color.r = bufferColor[0];
    color.g = bufferColor[1];
    color.b = bufferColor[2];
    color.a = bufferColor[3];
}

}