#ifndef BASE_API_HPP
#define BASE_API_HPP

#include "engine_api.hpp"
#include "comp/core/Transform.hpp"
#include "comp/core/ScriptUpdate.hpp"
#include "comp/core/ScriptCallback.hpp"
#include "res/asset/ScriptFile.hpp"

typedef mtrs::comp::Transform Transform;
typedef mtrs::comp::ScriptUpdate ScriptUpdate;
typedef mtrs::comp::ScriptCallback ScriptCallback;

extern mtrs::EngineAPI *api;

namespace mtrs::res
{
    void *ScriptFile::get_symbol(std::string &&name)
    {
        return api->script_get_symbol(this, name.c_str());
    }
}

template<typename Fun>
Fun ScriptCallback::get_fun_script(const char *name)
{
    return reinterpret_cast<Fun>(script_file->get_symbol(name));
}


#endif
