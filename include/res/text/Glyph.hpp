#ifndef GLYPH_HPP
#define GLYPH_HPP

#include "res/render/SubTexture.hpp"

namespace mtrs::res
{

struct Glyph
{
    char32_t symbol;
    SubTexture sub_texture;
    glm::vec2 size;
};

}

#endif
