#ifndef TEXT_HPP
#define TEXT_HPP

#include "res/text/Glyph.hpp"

#include <memory>
#include <string>
#include <vector>

namespace mtrs::res
{

class Font;

struct Text
{
    std::u32string string;
    std::vector<std::shared_ptr<res::Font>> fonts;
    std::vector<std::pair<size_t, std::vector<res::Glyph>>> glyphs;
    glm::uvec2 space_size;
};

}

#endif
