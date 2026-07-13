#ifndef SPRITE_API_HPP
#define SPRITE_API_HPP

#include "engine_api.hpp"
#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/SpriteMap.hpp"
#include "res/asset/TextureAtlas.hpp"

typedef mtrs::comp::Sprite Sprite;
typedef mtrs::comp::SpriteMap SpriteMap;
typedef mtrs::res::TextureAtlas TextureAtlas;

extern mtrs::EngineAPI *api;

TextureAtlas::SubTexture2D TextureAtlas::get_sub_texture(const size_t index) const
{
    return api->atlas_get_sub_texture(this, index);
}

#endif
