#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include "glm/vec2.hpp"

#include "res/asset/Asset.hpp"

#include <vector>

namespace mtrs::res
{

class ResourceManager;

class TextureAtlas
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
    ASSETS_CONSTRUCTORS(TextureAtlas);

    const SubTexture2D &get_sub_texture(const size_t index) const;
};

}

#endif
