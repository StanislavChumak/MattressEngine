#include "comp/rendering/Sprite.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"
#include "res/asset/TextureAtlas.hpp"

#include "util/func/files/data_mtrs_file.hpp"

#include <fstream>
#include <cstring>

#include "dynamic_field.def"
#include "comp_struct/Sprite.struct"

#ifndef FLAG_RELEASE
    #define SET_RESOURCE(res, Type, manager, scene, path) \
        if(path == "") { mtrs::util::mtrs_error("In scene \"",scene, \
            "\" resource type \"",#Type,"\" has an empty path"); } \
        if(!(res = manager.get_resource<Type>(scene, path))) \
        { mtrs::util::mtrs_error("In scene \"",scene, \
            "\" resource type \"",#Type,"\" received nothing along the way"); }
#else
    #define SET_RESOURCE(res, Type, manager, scene, path) \
        res = manager.get_resource<Type>(scene, path);
#endif

namespace mtrs::comp
{

Sprite::Sprite(COMPONENT_ARGS)
{
    Sprite_sc sprite;
    file.read(reinterpret_cast<char*>(&sprite), sizeof(sprite));

    std::string path_buffer;

    util::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite, shader));
    SET_RESOURCE(shader, res::ShaderProgram, resource, scene, path_buffer)

    util::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite, texture));
    SET_RESOURCE(texture, res::Texture, resource, scene, path_buffer)

    util::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite, atlas));
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
}

}