#ifndef TEXT_SUBMIT_SYSTEM_HPP
#define TEXT_SUBMIT_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::comp
{
    struct Render;
    struct GlyphDecoder;
}

namespace mtrs::sys
{

class TextSubmitSystem : public System<TextSubmitSystem>
{
public:
    static comp::Render *render;
    static comp::GlyphDecoder *decoder;
    
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::RENDERING; }
    static constexpr const char *get_system_name_imp() { return "TextSubmitSystem"; }
};

}

#endif
