#include "res/asset/ScriptFile.hpp"

#include "comp/ECSWorld.hpp"

#include "util/set_from_file_mtrs.hpp"
#include "util/hash.hpp"
#include "util/mtrs_message.hpp"

#include "dynamic_field.def"
#include "res_struct/ScriptFile.struct"

namespace mtrs::res
{

EngineAPI ScriptFile::_api{};

ScriptFile::ScriptFile(std::ifstream &file)
{
    ScriptFile_sc script_file;
    file.read(reinterpret_cast<char*>(&script_file), sizeof(script_file));

    std::string path;
    util::set_string_from_mtrs_file(file, path, DYNAMIC_ARGS(script_file, path));

    _handle = util::load_library(path + util::lib_extension());
    if (_handle)
    {
        _on_load = reinterpret_cast<decltype(_on_load)>(util::get_symbol(_handle, "on_load"));
        _on_unload = reinterpret_cast<decltype(_on_unload)>(util::get_symbol(_handle, "on_unload"));
        _update = reinterpret_cast<decltype(_update)>(util::get_symbol(_handle, "update"));
    }
    else
    {
        util::mtrs_message(util::TypeMessage::ERROR, "Failed to load script at path: ", path, "\n",
            util::get_last_error());
    }
}

ScriptFile::ScriptFile(ScriptFile &&other) noexcept
{
    _handle = other._handle;
    other._handle = nullptr;
    _on_load = other._on_load;
    other._on_load = nullptr;
    _on_unload = other._on_unload;
    other._on_unload = nullptr;
    _update = other._update;
    other._update = nullptr;
}

ScriptFile &ScriptFile::operator=(ScriptFile &&other) noexcept
{
    if(this != &other)
    {
        _handle = other._handle;
        other._handle = nullptr;
        _on_load = other._on_load;
        other._on_load = nullptr;
        _on_unload = other._on_unload;
        other._on_unload = nullptr;
        _update = other._update;
        other._update = nullptr;
    }
    return *this;
}

ScriptFile::~ScriptFile()
{
    if(_handle)
    {
        _on_unload();
        util::free_library(_handle);
        _handle = nullptr;
    }
}

std::string ScriptFile::get_type_name_imp() noexcept
{
    return "scripts";
}

uint32_t ScriptFile::get_type_size_imp() noexcept
{
    return sizeof(ScriptFile_sc);
}

void ScriptFile::update(comp::EntityID entity, const double &delta)
{
    _update(entity, delta);
}

void messege(const char *msg)
{
    util::mtrs_message(util::TypeMessage::LOG, msg);
}

void *get_component(comp::ECSWorld* world, uint64_t hash_comp, comp::EntityID entity)
{
    return world->script_get_component(hash_comp, entity);
}

void *get_single_component(comp::ECSWorld* world, uint64_t hash_comp)
{
    return world->script_get_single_comp(hash_comp);
}

void ScriptFile::load(comp::EntityID entity, comp::ECSWorld& world, ResourceManager& resource)
{
    if(!_api.init)
    {
        _api.world = &world;

        _api.messege = messege;
        _api.get_component = get_component;
        _api.get_single_component = get_single_component;

        _api.init = true;
    }
    _on_load(entity, &_api);
}

}