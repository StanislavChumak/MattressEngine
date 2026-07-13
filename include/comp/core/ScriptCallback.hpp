#ifndef SCRIPT_CALLBACK_HPP
#define SCRIPT_CALLBACK_HPP

#include "comp/Component.hpp"

#include <memory>

namespace mtrs::res
{
    class ScriptFile;
}

namespace mtrs::comp
{

struct ScriptCallback : public Component<ScriptCallback>
{
    std::shared_ptr<res::ScriptFile> script_file;

    ScriptCallback() = delete;
    ScriptCallback(COMPONENT_ARGS);

    template<typename Fun>
    Fun get_fun_script(const char *name);

    static constexpr const char *get_type_name_imp() noexcept { return "ScriptCallback"; }
};

}

#endif
