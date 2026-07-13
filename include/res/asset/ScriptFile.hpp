#ifndef SCRIPT_FILE_HPP
#define SCRIPT_FILE_HPP

#include "res/asset/Asset.hpp"
#include "util/files/load_external_lib.hpp"
#include "script/engine_api.hpp"

namespace mtrs::res
{
class ResourceManager;

class ScriptFile : public Asset<ScriptFile>
{
    EngineAPI _api;
    file::LibHandle _handle = nullptr;
    void (*_on_load)(mtrs::comp::EntityID, EngineAPI*) = nullptr;
    void (*_on_unload)() = nullptr;

public:
    ScriptFile(ASSET_ARGS);
    ScriptFile() = delete;
    ScriptFile(const ScriptFile&) = delete;
    ScriptFile &operator=(const ScriptFile&) = delete;
    ScriptFile(ScriptFile &&other) noexcept;
    ScriptFile &operator=(ScriptFile &&other) noexcept;
    ~ScriptFile();

    static std::string get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    void load(comp::EntityID entity, comp::ECSWorld& world, ResourceManager& resource);
    void *get_symbol(std::string &&name);
};

}

#endif
