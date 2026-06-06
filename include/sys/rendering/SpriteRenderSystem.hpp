#ifndef SPRITE_RENDER_SYSTEM_HPP
#define SPRITE_RENDER_SYSTEM_HPP

#include <memory>

namespace mtrs::res {
    class RenderContext;
}
namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class SpriteRenderSystem
{
public:
    static std::shared_ptr<res::RenderContext> context;
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
