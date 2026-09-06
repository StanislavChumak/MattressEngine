#ifndef STORED_DATA_HPP
#define STORED_DATA_HPP

#include "comp/Component.hpp"

#include <vector>

namespace mtrs::comp
{

struct StoredData : public Component<StoredData>
{
    std::vector<char> data;

    StoredData() = delete;
    StoredData(COMPONENT_ARGS);

    static constexpr const char *get_type_name_imp() noexcept { return "StoredData"; }
    static uint32_t get_prs_size_imp() noexcept;
};

}

#endif
