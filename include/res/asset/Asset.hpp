#ifndef ASSET_HPP
#define ASSET_HPP

#include <string>
#include <cstdint>

template<typename Derived>
class Asset
{
public:
    static std::string get_type_name() noexcept
    {
        return Derived::get_type_name_imp();
    }

    static uint32_t get_type_size() noexcept
    {
        return Derived::get_type_size_imp();
    }
};

#endif
