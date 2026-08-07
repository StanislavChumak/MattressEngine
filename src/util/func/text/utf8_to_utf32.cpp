#include "util/func/text/utf8_to_utf32.hpp"

namespace mtrs::util
{

std::u32string utf8_to_utf32(std::string_view text)
{
    std::u32string result;

    size_t i = 0;

    while (i < text.size())
    {
        uint8_t c = static_cast<uint8_t>(text[i]);

        if ((c & 0b10000000) == 0)
        {
            // 1 байт
            result.push_back(c);
            ++i;
        }
        else if ((c & 0b11100000) == 0b11000000)
        {
            // 2 байта
            char32_t cp =
                ((c & 0x1F) << 6) |
                (static_cast<uint8_t>(text[i + 1]) & 0x3F);

            result.push_back(cp);
            i += 2;
        }
        else if ((c & 0b11110000) == 0b11100000)
        {
            // 3 байта
            char32_t cp =
                ((c & 0x0F) << 12) |
                ((static_cast<uint8_t>(text[i + 1]) & 0x3F) << 6) |
                (static_cast<uint8_t>(text[i + 2]) & 0x3F);

            result.push_back(cp);
            i += 3;
        }
        else
        {
            // 4 байта
            char32_t cp =
                ((c & 0x07) << 18) |
                ((static_cast<uint8_t>(text[i + 1]) & 0x3F) << 12) |
                ((static_cast<uint8_t>(text[i + 2]) & 0x3F) << 6) |
                (static_cast<uint8_t>(text[i + 3]) & 0x3F);

            result.push_back(cp);
            i += 4;
        }
    }

    return result;
}

}