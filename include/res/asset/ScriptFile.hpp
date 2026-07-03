#ifndef SCRIPT_FILE_HPP
#define SCRIPT_FILE_HPP

#include "res/asset/Asset.hpp"
#include "util/util_for_external_lib.hpp"

namespace mtrs::comp
{
    class ECSWorld;
    typedef uint64_t EntityID;
}

namespace mtrs
{
    struct EngineAPI
    {
        bool init = false;
        comp::ECSWorld *world;
        void (*messege)(const char *) = nullptr;
        void *(*get_component)(comp::ECSWorld*, uint64_t, comp::EntityID) = nullptr;
        void *(*get_single_component)(comp::ECSWorld*, uint64_t) = nullptr;
    };
}

namespace mtrs::res
{
class ResourceManager;

class ScriptFile : public Asset<ScriptFile>
{
    static EngineAPI _api;
    util::LibHandle _handle = nullptr;
    void (*_on_load)(mtrs::comp::EntityID, EngineAPI*) = nullptr;
    void (*_on_unload)() = nullptr;
    void (*_update)(comp::EntityID, const double&) = nullptr;

public:
    ScriptFile(std::ifstream &file);
    ScriptFile() = delete;
    ScriptFile(const ScriptFile&) = delete;
    ScriptFile &operator=(const ScriptFile&) = delete;
    ScriptFile(ScriptFile &&other) noexcept;
    ScriptFile &operator=(ScriptFile &&other) noexcept;
    ~ScriptFile();

    static std::string get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    void update(comp::EntityID entity, const double &delta);
    void load(comp::EntityID entity, comp::ECSWorld& world, ResourceManager& resource);
};

}

#endif
