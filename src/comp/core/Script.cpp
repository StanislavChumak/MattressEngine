#include "comp/core/Script.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ScriptFile.hpp"

#include "util/set_from_file_mtrs.hpp"

#include "dynamic_field.def"
#include "comp_struct/Script.struct"

namespace mtrs::comp
{

Script::Script(COMPONENT_ARGS)
{
    Script_sc script;
    file.read(reinterpret_cast<char*>(&script), sizeof(script));

    std::string path;
    util::set_string_from_mtrs_file(file, path, DYNAMIC_ARGS(script, script_file));

    script_file = resource.get_resource<res::ScriptFile>(path);
    script_file->load(entity, world, resource);
}

}