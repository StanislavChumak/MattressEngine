#ifndef SPRITE_MAP_HPP
#define SPRITE_MAP_HPP

#include "comp/Component.hpp"
#include "res/render/SubTexture.hpp"

#include "glm/vec4.hpp"

#include <memory>

namespace mtrs::res
{
    class Texture;
    class ShaderProgram;
    class TextureAtlas;
}

namespace mtrs::comp
{

struct SpriteMap : public Component<SpriteMap>
{
    std::shared_ptr<res::ShaderProgram> shader;
    std::shared_ptr<res::Texture> texture;
    std::shared_ptr<res::TextureAtlas> atlas;

    std::vector<res::SubTexture> cell_types;

    struct MapCell
    {
        size_t type;
        glm::ivec2 cord;
    };
    std::vector<MapCell> cell_map;

    glm::uvec2 cell_size;
    glm::tvec4<uint8_t, glm::precision::highp> color;
    float layer;

    bool visibility;

    SpriteMap() = delete;
    SpriteMap(COMPONENT_ARGS);

    static constexpr const char *get_type_name_imp() noexcept { return "SpriteMap"; }
    static uint32_t get_prs_size_imp() noexcept;
};

}

#endif
