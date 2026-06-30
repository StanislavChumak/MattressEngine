#ifndef GET_FROM_FILE_MTRS_HPP
#define GET_FROM_FILE_MTRS_HPP

#include <string>
#include <cstdint>

#include <fstream>

#define DYNAMIC_ARGS(struct, field) (struct).field##_offset, (struct).field##_size 

namespace mtrs::util
{

//std::string get_string_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size);

void set_string_from_mtrs_file(std::ifstream &file, std::string &dest, uint32_t offset, uint32_t size);

// template<typename T>
// T *get_array_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size)
// {
//     T *result = new T[size / sizeof(T)];
//     std::streampos pos_buffer = file.tellg();

//     file.seekg(offset, std::ios::beg);

//     file.read(reinterpret_cast<char*>(result), size);

//     file.seekg(pos_buffer, std::ios::beg);

//     return result;
// }

template<typename T>
void set_array_from_mtrs_file(std::ifstream &file, T& dest, uint32_t offset, uint32_t size)
{
    std::streampos pos_buffer = file.tellg();

    file.seekg(offset, std::ios::beg);

    dest.resize(size / sizeof(dest[0]));
    file.read(reinterpret_cast<char*>(dest.data()), size);

    file.seekg(pos_buffer, std::ios::beg);
}

}

#endif
