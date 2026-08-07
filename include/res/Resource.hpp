#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>
#include <iosfwd>

namespace mtrs::res
{
    class ResourceManager;
}

template<typename Derived>
class Resource
{
public:
    static const char *get_type_name() noexcept
    {
        return Derived::get_type_name_imp();
    }

    static uint32_t get_type_size() noexcept
    {
        return Derived::get_type_size_imp();
    }
};

#define RESOURCE_ARGS const std::string &dir_pack, const std::string &pack, \
    std::ifstream &file, mtrs::res::ResourceManager &resources

#endif
