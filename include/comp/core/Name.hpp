#ifndef NAME_HPP
#define NAME_HPP

#include "Component.hpp"

#include <string>

namespace mtrs::res
{
    class ResourceManager;
}

namespace mtrs::comp
{

struct Name
{
    std::string value;

    Name(COMPONENT_ARGS);
};

}

#endif
