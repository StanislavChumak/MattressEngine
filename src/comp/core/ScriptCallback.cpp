#include "comp/core/ScriptCallback.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ScriptFile.hpp"

#include "util/files/data_mtrs_file.hpp"

#include "dynamic_field.def"
#include "comp_struct/ScriptCallback.struct"

namespace mtrs::comp
{

ScriptCallback::ScriptCallback(COMPONENT_ARGS)
{
    ScriptCallback_sc script;
    file.read(reinterpret_cast<char*>(&script), sizeof(script));

    std::string path;
    file::set_string_from_mtrs_file(file, path, DYNAMIC_ARGS(script, script_file));

    script_file = resource.get_resource<res::ScriptFile>(path);
    script_file->load(entity, world, resource);
}

}