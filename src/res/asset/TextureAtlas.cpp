#include "res/asset/TextureAtlas.hpp"

#include "dynamic_field.def"
#include "res_struct/TextureAtlas.struct"
#include "util/mtrs_message.hpp"

#include <fstream>

namespace mtrs::res
{

TextureAtlas::TextureAtlas(std::ifstream &file)
{
    TextureAtlas_rs atlas;
    file.read(reinterpret_cast<char*>(&atlas), sizeof(atlas));

    uint32_t count = (atlas.height / atlas.sub_height) * (atlas.width / atlas.sub_width);
    uint64_t current_offset_x = 0;
    uint64_t current_offset_y = atlas.height;
    _atlas.clear();
    for(size_t i = 0; i < count; i++)
    {
        glm::vec2 left_bottom(static_cast<float>(current_offset_x + 0.01f) / atlas.width,
            static_cast<float>(current_offset_y - atlas.sub_height + 0.01f) / atlas.height);
        glm::vec2 rigth_top(static_cast<float>(current_offset_x + atlas.sub_width - 0.01f) / atlas.width, 
            static_cast<float>(current_offset_y - 0.01f) / atlas.height);

        _atlas.push_back(SubTexture2D{left_bottom, rigth_top});

        current_offset_x += atlas.sub_width;
        if (current_offset_x >= atlas.width)
        {
            current_offset_x = 0;
            current_offset_y -= atlas.sub_height;
        }
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

const TextureAtlas::SubTexture2D &TextureAtlas::get_sub_texture(const size_t index) const
{
    if (index >= _atlas.size())
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Fatal find subTexture \"", index , "\"");
        return _atlas[0];
    }
    return _atlas[index];
}

}