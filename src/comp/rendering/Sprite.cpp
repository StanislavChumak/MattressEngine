#include "comp/rendering/Sprite.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"
#include "res/asset/TextureAtlas.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/Sprite.hpp"

#include <fstream>
#include <cstring>

#ifndef FLAG_RELEASE
    #define SET_RESOURCE(res, Type, manager, scene, path) \
        if(path == "") { mtrs::msg::mtrs_error("In scene \"",scene, \
            "\" resource type \"",#Type,"\" has an empty path"); } \
        if(!(res = manager.get_resource<Type>(scene, path))) \
        { mtrs::msg::mtrs_error("In scene \"",scene, \
            "\" resource type \"",#Type,"\" received nothing along the way"); }
#else
    #define SET_RESOURCE(res, Type, manager, scene, path) \
        res = manager.get_resource<Type>(scene, path);
#endif

namespace mtrs::comp
{

Sprite::Sprite(COMPONENT_ARGS)
{
    prs::Sprite sprite;
    file.read(reinterpret_cast<char*>(&sprite), sizeof(sprite));

    std::string path_buffer;

    prs::set_mtrs_to_var(file, path_buffer, DEFERRED_ARGS(sprite, shader));
    SET_RESOURCE(shader, res::ShaderProgram, resource, scene, path_buffer)

    prs::set_mtrs_to_var(file, path_buffer, DEFERRED_ARGS(sprite, texture));
    SET_RESOURCE(texture, res::Texture, resource, scene, path_buffer)

    prs::set_mtrs_to_var(file, path_buffer, DEFERRED_ARGS(sprite, atlas));
    if(path_buffer != "")
    {
        SET_RESOURCE(atlas, res::TextureAtlas, resource, scene, path_buffer)
        sub_texture = atlas->get_sub_texture(0);
    }
    else
    {
        atlas = nullptr; 
        sub_texture = res::SubTexture();
    }

    layer = sprite.layer;

    size.x = sprite.size_x;
    size.y = sprite.size_y;

    std::memcpy(&color, &sprite.color, 4);

    visibility = sprite.visibility;
}

}