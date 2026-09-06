#include "comp/core/StoredData.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/StoredData.hpp"

#include <cstring>

namespace mtrs::comp
{

StoredData::StoredData(COMPONENT_ARGS)
{
    prs::StoredData stored;
    std::memcpy(&stored, file_data, sizeof(stored));

    prs::set_mtrs_to_var(file_ddata[stored.data], data);
}

uint32_t StoredData::get_prs_size_imp() noexcept
{
    return sizeof(prs::StoredData);
}

}