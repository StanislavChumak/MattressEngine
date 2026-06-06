#ifndef PARENT_CHILDREN_HPP
#define PARENT_CHILDREN_HPP

#include "comp/Component.hpp"

#include <vector>

namespace mtrs::res
{
    class ResourceManager;
}

namespace mtrs::comp
{

struct Children
{
    std::vector<EntityID> childrensIds;
};

struct Parent
{
    EntityID parentId;

    Parent(COMPONENT_ARGS);
};

}

#endif
