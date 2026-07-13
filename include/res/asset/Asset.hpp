#ifndef ASSET_HPP
#define ASSET_HPP

#include <string>
#include <cstdint>
#include <iosfwd>

template<typename Derived>
class Asset
{
public:
    static constexpr std::string get_type_name() noexcept
    {
        return Derived::get_type_name_imp();
    }

    static constexpr uint32_t get_type_size() noexcept
    {
        return Derived::get_type_size_imp();
    }
};

#define ASSET_ARGS std::ifstream &file, const std::string& dir_resource

#endif
