#ifndef UTF8_TO_UTF32_HPP
#define UTF8_TO_UTF32_HPP

#include <string>
#include <cstdint>

namespace mtrs::util
{

std::u32string utf8_to_utf32(std::string_view text);

}

#endif
