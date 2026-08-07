#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::comp
{
    struct Render;
}

namespace mtrs::sys
{

class RenderSystem : public System<RenderSystem>
{
public:
    static comp::Render *render;
    
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::RENDERING + 100; }
    static constexpr const char *get_system_name_imp() { return "RenderSystem"; }
};

}

#endif
