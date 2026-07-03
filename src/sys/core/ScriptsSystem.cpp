#include "sys/core/ScriptsSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/core/Script.hpp"
#include "res/asset/ScriptFile.hpp"

#include "comp/single/KeyButtons.hpp"

namespace mtrs::sys
{

void ScriptsSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    comp::KeyButtons *key = world.single_comp<comp::KeyButtons>();
    for(auto [entity, script] : world.view<comp::Script>())
    {
        script->script_file->update(entity, delta);
    }
}

}