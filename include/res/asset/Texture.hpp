#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "res/asset/Asset.hpp"

#include <iosfwd>

namespace mtrs::res
{

class Texture : public Asset<Texture>
{
    uint32_t _ID;
    uint8_t _number;
    int32_t _mode;
    int32_t _width;
    int32_t _height;

public:
    Texture(std::ifstream &file);
    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture &operator=(const Texture&) = delete;
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;
    ~Texture();

    static std::string get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    void bind() const;
    void active() const;
    uint32_t id() const noexcept;
};

}

#endif
