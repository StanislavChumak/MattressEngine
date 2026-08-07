#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include "res/Resource.hpp"
#include "res/render/SubTexture.hpp"

#include <vector>

namespace mtrs::res
{

class ResourceManager;

class TextureAtlas : public Resource<TextureAtlas>
{
    std::vector<SubTexture> _atlas;

public:
    TextureAtlas(RESOURCE_ARGS);
    TextureAtlas() = delete;
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas &operator=(const TextureAtlas&) = delete;
    TextureAtlas(TextureAtlas &&other) noexcept;
    TextureAtlas &operator=(TextureAtlas &&other) noexcept;
    ~TextureAtlas();

    static const char *get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    SubTexture get_sub_texture(const size_t index) const;
};

}

#endif
