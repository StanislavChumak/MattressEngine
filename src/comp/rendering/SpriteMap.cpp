#include "comp/rendering/SpriteMap.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/RenderContext.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include "util/files/data_mtrs_file.hpp"

#include <fstream>
#include <cstring>

#include "dynamic_field.def"
#include "comp_struct/SpriteMap.struct"

namespace mtrs::comp
{

SpriteMap::SpriteMap(COMPONENT_ARGS)
{
    SpriteMap_sc sprite_map;
    file.read(reinterpret_cast<char*>(&sprite_map), sizeof(sprite_map));

    std::string path_buffer;

    file::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite_map, shader));
    shader = resource.get_resource<res::ShaderProgram>(scene, path_buffer);

    file::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite_map, texture));
    texture = resource.get_resource<res::Texture>(scene, path_buffer);

    file::set_string_from_mtrs_file(file, path_buffer, DYNAMIC_ARGS(sprite_map, atlas));
    atlas = resource.get_resource<res::TextureAtlas>(scene, path_buffer);

    auto context = resource.get_resource<res::RenderContext>("system", "render_context");
    context->create_sprite_batch(shader, texture);

    std::vector<uint32_t> buffer_types;
    file::set_array_from_mtrs_file(file, buffer_types, DYNAMIC_ARGS(sprite_map, cell_types));
    cell_types.resize(buffer_types.size());

    for(size_t i = 0; i < buffer_types.size(); i++)
    {
        cell_types[i] = atlas->get_sub_texture(buffer_types[i]);
    }

    std::vector<SpriteMap_sc::MapCell> map_buffer;
    file::set_array_from_mtrs_file(file, map_buffer, DYNAMIC_ARGS(sprite_map, cell_map));
    cell_map.reserve(map_buffer.size());
    for(auto &cell : map_buffer)
    {
        cell_map.push_back({cell.type, {cell.cord_x, cell.cord_y}});
    }

    layer = sprite_map.layer;

    cell_size.x = sprite_map.cell_size_x;
    cell_size.y = sprite_map.cell_size_y;

    std::memcpy(&color, &sprite_map.color, 4);
}

}