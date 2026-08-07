#include "sys/rendering/RenderSystem.hpp"

#include "comp/single/Render.hpp"

namespace mtrs::sys
{

comp::Render *RenderSystem::render;

void RenderSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    render->draw();
}

}