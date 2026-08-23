#include "comp/core/ScriptUpdate.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ScriptFile.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/ScriptUpdate.hpp"

namespace mtrs::comp
{

ScriptUpdate::ScriptUpdate(COMPONENT_ARGS)
{
    prs::ScriptUpdate script;
    file.read(reinterpret_cast<char*>(&script), sizeof(script));

    std::string path;
    prs::set_mtrs_to_var(file, path, DEFERRED_ARGS(script, script_file));

    script_file = resource.get_resource<res::ScriptFile>(scene, path);
    script_file->load(scene, entity, world, resource);
    
    update = reinterpret_cast<decltype(update)>(script_file->get_symbol("update"));
}

}