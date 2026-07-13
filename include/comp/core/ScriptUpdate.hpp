#ifndef SCRIPT_UPDATE_HPP
#define SCRIPT_UPDATE_HPP

#include "comp/Component.hpp"

#include <memory>

namespace mtrs::res
{
    class ScriptFile;
}

namespace mtrs::comp
{

struct ScriptUpdate : public Component<ScriptUpdate>
{
    std::shared_ptr<res::ScriptFile> script_file;
    void (*update)(comp::EntityID, const double&) = nullptr;

    ScriptUpdate() = delete;
    ScriptUpdate(COMPONENT_ARGS);

    static constexpr const char *get_type_name_imp() noexcept { return "ScriptUpdate"; }
};

}

#endif
