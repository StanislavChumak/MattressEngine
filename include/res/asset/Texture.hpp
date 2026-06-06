#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "res/asset/Asset.hpp"

namespace mtrs::res
{

class Texture
{
    uint32_t _ID;
    uint8_t _number;
    int32_t _mode;
    int32_t _width;
    int32_t _height;

public:
    ASSETS_CONSTRUCTORS(Texture);

    void bind() const;
    void active() const;
    uint32_t id() const noexcept;
};

}

#endif
