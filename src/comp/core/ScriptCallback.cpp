#include "comp/core/ScriptCallback.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ScriptFile.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/ScriptCallback.hpp"

namespace mtrs::comp
{

ScriptCallback::ScriptCallback(COMPONENT_ARGS)
{
    prs::ScriptCallback script;
    file.read(reinterpret_cast<char*>(&script), sizeof(script));

    std::string path;
    prs::set_mtrs_to_var(file, path, DEFERRED_ARGS(script, script_file));

    script_file = resource.get_resource<res::ScriptFile>(scene, path);
    script_file->load(scene, entity, world, resource);
}

}