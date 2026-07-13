#include "res/asset/TextureAtlas.hpp"

#include "dynamic_field.def"
#include "res_struct/TextureAtlas.struct"
#include "util/mtrs_message.hpp"

#include <fstream>

namespace mtrs::res
{

std::vector<TextureAtlas::SubTexture2D> get_classic_atlas(TextureAtlas_rs atlas)
{
    std::vector<TextureAtlas::SubTexture2D> sub_textures;

    uint64_t current_offset_x = 0;
    uint64_t current_offset_y = atlas.height;
    uint32_t count = (atlas.height / atlas.sub_height) * (atlas.width / atlas.sub_width);

    sub_textures.clear();
    sub_textures.reserve(count);
    for(size_t i = 0; i < count; i++)
    {
        glm::vec2 left_bottom(static_cast<float>(current_offset_x + 0.01f) / atlas.width,
            static_cast<float>(current_offset_y - atlas.sub_height + 0.01f) / atlas.height);
        glm::vec2 rigth_top(static_cast<float>(current_offset_x + atlas.sub_width - 0.01f) / atlas.width, 
            static_cast<float>(current_offset_y - 0.01f) / atlas.height);

        sub_textures.push_back(TextureAtlas::SubTexture2D{left_bottom, rigth_top});

        current_offset_x += atlas.sub_width;
        if (current_offset_x >= atlas.width)
        {
            current_offset_x = 0;
            current_offset_y -= atlas.sub_height;
        }
    }

    return sub_textures;
}

std::vector<TextureAtlas::SubTexture2D> get_spiral_atlas(TextureAtlas_rs atlas)
{
    std::vector<TextureAtlas::SubTexture2D> sub_textures;

    uint64_t current_offset_x = 0;
    uint64_t current_offset_y = atlas.height;

    enum class Direction
    {
        Right,
        Left,
        Top,
        Botton
    };

    Direction direction = Direction::Right;

    sub_textures.clear();
    while (atlas.sub_width > 0 && atlas.sub_height > 0)
    {
        glm::vec2 left_bottom(static_cast<float>(current_offset_x + 0.01f) / atlas.width,
            static_cast<float>(current_offset_y - atlas.sub_height + 0.01f) / atlas.height);
        glm::vec2 rigth_top(static_cast<float>(current_offset_x + atlas.sub_width - 0.01f) / atlas.width, 
            static_cast<float>(current_offset_y - 0.01f) / atlas.height);

        sub_textures.push_back(TextureAtlas::SubTexture2D{left_bottom, rigth_top});

        switch (direction)
        {
        case Direction::Right:
            current_offset_x += atlas.sub_width;
            direction = Direction::Botton;
            break;
        case Direction::Botton:
            current_offset_x += atlas.sub_width / 2;
            current_offset_y -= atlas.sub_height;
            direction = Direction::Left;
            break;
        case Direction::Left:
            current_offset_x -= atlas.sub_width / 2;
            current_offset_y -= atlas.sub_height / 2;
            direction = Direction::Top;
            break;
        case Direction::Top:
            current_offset_y += atlas.sub_height / 2;
            direction = Direction::Right;
            break;
        }

        atlas.sub_height /= 2;
        atlas.sub_width /= 2;
    }
    
    return sub_textures;
}

TextureAtlas::TextureAtlas(ASSET_ARGS)
{
    TextureAtlas_rs atlas;
    file.read(reinterpret_cast<char*>(&atlas), sizeof(atlas));

    if(atlas.spirality)
    {
        _atlas = get_spiral_atlas(atlas);
    }
    else
    {
        _atlas = get_classic_atlas(atlas);
    }
}

TextureAtlas::TextureAtlas(TextureAtlas &&other) noexcept
{
    _atlas = other._atlas;
    other._atlas.clear();
}

TextureAtlas &TextureAtlas::operator=(TextureAtlas &&other) noexcept
{
    if(this != &other)
    {
        _atlas = other._atlas;
        other._atlas.clear();
    }
    return *this;
}

TextureAtlas::~TextureAtlas()
{
    _atlas.clear();
}

std::string TextureAtlas::get_type_name_imp() noexcept
{
    return "atlases";
}

uint32_t TextureAtlas::get_type_size_imp() noexcept
{
    return sizeof(TextureAtlas_rs);
}

TextureAtlas::SubTexture2D TextureAtlas::get_sub_texture(const size_t index) const
{
    if (index >= _atlas.size())
    {
        util::mtrs_error("Fatal find subTexture \"", index , "\"");
        return _atlas[0];
    }
    return _atlas[index];
}

}