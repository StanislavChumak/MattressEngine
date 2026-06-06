#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iosfwd>
#include <cstdint>

namespace mtrs::res
{
    class ResourceManager;
}
    
namespace mtrs::comp
{
    typedef uint64_t EntityID;
    class ECSWorld;
}

#define COMPONENT_ARGS mtrs::comp::EntityID entity, std::ifstream &file,\
    mtrs::comp::ECSWorld& world, mtrs::res::ResourceManager& resource    

#endif
