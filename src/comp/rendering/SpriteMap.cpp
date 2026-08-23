#include "comp/rendering/SpriteMap.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"
#include "res/asset/TextureAtlas.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/SpriteMap.hpp"

#include <fstream>
#include <cstring>

namespace mtrs::comp
{

SpriteMap::SpriteMap(COMPONENT_ARGS)
{
    prs::SpriteMap sprite_map;
    file.read(reinterpret_cast<char*>(&sprite_map), sizeof(sprite_map));

    std::string path_buffer;

    prs::set_mtrs_to_var(file, path_buffer, DEFERRED_ARGS(sprite_map, shader));
    shader = resource.get_resource<res::ShaderProgram>(scene, path_buffer);

    prs::set_mtrs_to_var(file, path_buffer, DEFERRED_ARGS(sprite_map, texture));
    texture = resource.get_resource<res::Texture>(scene, path_buffer);

    prs::set_mtrs_to_var(file, path_buffer, DEFERRED_ARGS(sprite_map, atlas));
    atlas = resource.get_resource<res::TextureAtlas>(scene, path_buffer);

    std::vector<uint32_t> buffer_types;
    prs::set_mtrs_to_var(file, buffer_types, DEFERRED_ARGS(sprite_map, cell_types));
    cell_types.resize(buffer_types.size());

    for(size_t i = 0; i < buffer_types.size(); i++)
    {
        cell_types[i] = atlas->get_sub_texture(buffer_types[i]);
    }

    std::vector<prs::SpriteMap::MapCell> map_buffer;
    prs::set_mtrs_to_var(file, map_buffer, DEFERRED_ARGS(sprite_map, cell_map));
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