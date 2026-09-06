#ifndef FONT_HPP
#define FONT_HPP

#include "res/Resource.hpp"
#include "res/text/Glyph.hpp"

#include "glm/vec2.hpp"

#include <vector>
#include <memory>
#include <string>

namespace mtrs::res
{
    
class SubTexture;
class Texture;

class Font : public Resource<Font>
{
    std::u32string _symbols;
    std::vector<glm::uvec2> _symbol_widths;
    uint32_t _symbol_height;
    
public:
    std::shared_ptr<Texture> texture;
    
    Font(RESOURCE_ARGS);
    Font() = delete;
    Font(const Font&) = delete;
    Font &operator=(const Font&) = delete;
    Font(Font &&other) noexcept;
    Font &operator=(Font &&other) noexcept;
    ~Font() = default;

    static constexpr const char *get_type_name_imp() noexcept { return "Font"; }
    static uint32_t get_prs_size_imp() noexcept;

    class Iterator
    {
        friend Font;

        const std::shared_ptr<Texture> &_texture;
        const std::u32string &_symbols;
        const std::vector<glm::uvec2> &_symbol_widths;
        const uint32_t &_symbol_height;

        size_t _index = 0;
        uint32_t _current_width;
        glm::uvec2 _current_offset;

        Iterator(const std::shared_ptr<Texture> &texture, const std::u32string &symbols,
            const std::vector<glm::uvec2> &symbol_widths, const uint32_t &symbol_height);
    public:

        bool operator!=(const Iterator& other) const noexcept;
        Iterator& operator++();
        Glyph operator*() const;
    };

    Iterator begin() const;
    Iterator end() const;
};

}

#endif
