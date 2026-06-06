#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "glm/vec4.hpp"

#include "comp/Component.hpp"
#include "res/asset/TextureAtlas.hpp"

#include <memory>

namespace mtrs::res
{
    class Texture;
    class ShaderProgram;
}

namespace mtrs::comp
{

struct Sprite
{
    std::shared_ptr<res::ShaderProgram> shader;
    std::shared_ptr<res::Texture> texture;
    std::shared_ptr<res::TextureAtlas> atlas;

    res::TextureAtlas::SubTexture2D sub_texture;
    glm::tvec4<uint8_t, glm::precision::highp> color{255, 255, 255, 255};
    glm::vec2 size;
    float layer = 0.f;

    bool visibility =  true;

    Sprite() = delete;
    Sprite(COMPONENT_ARGS);
};

}

#endif
