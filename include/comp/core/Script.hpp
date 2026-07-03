#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "comp/Component.hpp"

#include <memory>

namespace mtrs::res
{
    class ScriptFile;
}

namespace mtrs::comp
{

struct Script
{
    std::shared_ptr<res::ScriptFile> script_file;

    Script() = delete;
    Script(COMPONENT_ARGS);
};

}

#endif
