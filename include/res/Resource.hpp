#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>
#include <string>
#include <unordered_map>

namespace mtrs::res
{
    class ResourceManager;
}

namespace mtrs::prs
{
    struct DeferredData;
}

template<typename Derived>
class Resource
{
public:
    static constexpr const char *get_type_name() noexcept
    {
        return Derived::get_type_name_imp();
    }

    static uint32_t get_prs_size() noexcept
    {
        return Derived::get_prs_size_imp();
    }
};

#define RESOURCE_ARGS const std::string &dir_pack, const std::string &pack, \
    char *file_data, std::unordered_map<uint64_t, mtrs::prs::DeferredData> &file_ddata, \
    mtrs::res::ResourceManager &resources

#endif
