#include "sys/core/ScriptsSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/core/ScriptUpdate.hpp"

namespace mtrs::sys
{

void ScriptsSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, script] : world.view<comp::ScriptUpdate>())
    {
        script->update(entity, delta);
    }
}

}