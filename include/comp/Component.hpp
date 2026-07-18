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

template<typename Derived>
class Component
{
public:
    static constexpr const char *get_type_name() noexcept
    {
        return Derived::get_type_name_imp();
    }
};

#define COMPONENT_ARGS mtrs::comp::EntityID entity, const char* scene,\
    std::ifstream &file, mtrs::comp::ECSWorld& world, mtrs::res::ResourceManager& resource    

#endif
