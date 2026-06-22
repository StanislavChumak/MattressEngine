#ifndef SPRITE_RENDER_SYSTEM_HPP
#define SPRITE_RENDER_SYSTEM_HPP

#include "sys/System.hpp"

#include <memory>

namespace mtrs::res {
    class RenderContext;
}

namespace mtrs::sys
{

class SpriteRenderSystem : public System<SpriteRenderSystem>
{
public:
    static std::shared_ptr<res::RenderContext> context;
    
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::RENDERING; };
};

}

#endif
