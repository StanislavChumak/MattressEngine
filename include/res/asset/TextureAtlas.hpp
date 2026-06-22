#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include "glm/vec2.hpp"

#include "res/asset/Asset.hpp"

#include <iosfwd>
#include <vector>

namespace mtrs::res
{

class ResourceManager;

class TextureAtlas : public Asset<TextureAtlas>
{
public:
    struct SubTexture2D
    {
        glm::vec2 lb_vertex = glm::vec2(0.0f);
        glm::vec2 rt_vertex = glm::vec2(1.0f);
    };

private:
    std::vector<SubTexture2D> _atlas;

public:
    TextureAtlas(std::ifstream &file);
    TextureAtlas() = delete;
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas &operator=(const TextureAtlas&) = delete;
    TextureAtlas(TextureAtlas &&other) noexcept;
    TextureAtlas &operator=(TextureAtlas &&other) noexcept;
    ~TextureAtlas();

    static std::string get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    const SubTexture2D &get_sub_texture(const size_t index) const;
};

}

#endif
