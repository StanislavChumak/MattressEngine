#ifndef REGISTER_COMPONENT_HPP
#define REGISTER_COMPONENT_HPP

#include <iosfwd>
#include <functional>
#include <string>
#include <cstdint>

namespace mtrs::res
{
    class ResourceManager;
}

namespace mtrs::comp
{

typedef uint64_t EntityID;
class ECSWorld;

typedef std::function<void(EntityID,std::ifstream&,ECSWorld&,res::ResourceManager&)> TypeInfo;

inline std::unordered_map<std::string, TypeInfo> type_registry;

#define REGISTER_COMPONENT(Component) \
type_registry[#Component] = { [](EntityID entity, std::ifstream &file,      \
                            ECSWorld &world, res::ResourceManager &resource)\
{                                                                           \
    Component c(entity, file, world, resource);                             \
    world.add_comp<Component>(entity, std::move(c));                        \
}}

}

#endif
