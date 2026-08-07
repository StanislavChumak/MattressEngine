#ifndef SPRITE_SUBMIT_SYSTEM_HPP
#define SPRITE_SUBMIT_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::comp
{
    struct Render;
}

namespace mtrs::sys
{

class SpriteSubmitSystem : public System<SpriteSubmitSystem>
{
public:
    static comp::Render *render;
    
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::RENDERING; }
    static constexpr const char *get_system_name_imp() { return "SpriteSubmitSystem"; }
};

}

#endif
