#include "comp/core/ScriptUpdate.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ScriptFile.hpp"

#include "util/func/files/data_mtrs_file.hpp"

#include "dynamic_field.def"
#include "comp_struct/ScriptUpdate.struct"

namespace mtrs::comp
{

ScriptUpdate::ScriptUpdate(COMPONENT_ARGS)
{
    ScriptUpdate_sc script;
    file.read(reinterpret_cast<char*>(&script), sizeof(script));

    std::string path;
    util::set_string_from_mtrs_file(file, path, DYNAMIC_ARGS(script, script_file));

    script_file = resource.get_resource<res::ScriptFile>(scene, path);
    script_file->load(scene, entity, world, resource);
    
    update = reinterpret_cast<decltype(update)>(script_file->get_symbol("update"));
}

}