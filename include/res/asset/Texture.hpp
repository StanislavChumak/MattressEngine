#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "res/Resource.hpp"

namespace mtrs::res
{

class Texture : public Resource<Texture>
{
    uint32_t _ID;
    uint8_t _number;
    int32_t _mode;
    int _width;
    int _height;
    int64_t _max_instances;

public:
    Texture(RESOURCE_ARGS);
    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture &operator=(const Texture&) = delete;
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;
    ~Texture();

    static const char *get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    void bind() const;
    void active() const;
    
    uint32_t id() const noexcept;
    uint64_t max_instances() const noexcept;
    int width() const noexcept;
    int height() const noexcept;
};

}

#endif
