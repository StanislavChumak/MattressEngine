#ifndef SPRITE_HPP
#define SPRITE_HPP

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

struct Sprite : public Component<Sprite>
{
    std::shared_ptr<res::ShaderProgram> shader;
    std::shared_ptr<res::Texture> texture;
    std::shared_ptr<res::TextureAtlas> atlas;

    res::SubTexture sub_texture;
    
    glm::uvec2 size;
    glm::tvec4<uint8_t, glm::precision::highp> color;
    float layer;

    bool visibility;

    Sprite() = delete;
    Sprite(COMPONENT_ARGS);

    static constexpr const char *get_type_name_imp() noexcept { return "Sprite"; }
    static uint32_t get_prs_size_imp() noexcept;
};

}

#endif
