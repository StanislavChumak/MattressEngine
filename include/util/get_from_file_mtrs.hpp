#ifndef GET_FROM_FILE_MTRS_HPP
#define GET_FROM_FILE_MTRS_HPP

#include <string>
#include <cstdint>
#include <iosfwd>

#define DYNAMIC_ARGS(struct, field) (struct).field##_offset, (struct).field##_size 

namespace mtrs::util
{

std::string get_string_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size);

template<typename T>
T *get_array_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size);

}

#endif
