#include "comp/rendering/Sprite.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/RenderContext.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "util/files/data_mtrs_file.hpp"

#include <fstream>
#include <cstring>

#include "dynamic_field.def"
#include "comp_struct/Sprite.struct"

namespace mtrs::comp
{

Sprite::Sprite(COMPONENT_ARGS)
{
    Sprite_sc sprite;
    file.read(reinterpret_cast<char*>(&sprite), sizeof(sprite));

    std::string path_buffer;

    file::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite, shader));
    shader = resource.get_resource<res::ShaderProgram>(scene, path_buffer);

    file::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite, texture));
    texture = resource.get_resource<res::Texture>(scene, path_buffer);

    file::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite, atlas));
    
    if(path_buffer != "")
    {
        atlas = resource.get_resource<res::TextureAtlas>(scene, path_buffer);
        sub_texture = atlas->get_sub_texture(0);
    }
    else
    {
        atlas = nullptr; 
        sub_texture = res::TextureAtlas::SubTexture2D();
    }

    auto context = resource.get_resource<res::RenderContext>("system", "render_context");
    context->create_sprite_batch(shader, texture);

    layer = sprite.layer;

    size.x = sprite.size_x;
    size.y = sprite.size_y;

    std::memcpy(&color, &sprite.color, 4);
}

}