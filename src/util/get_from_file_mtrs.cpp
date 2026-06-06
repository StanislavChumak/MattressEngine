#include "util/get_from_file_mtrs.hpp"

#include <fstream>

namespace mtrs::util
{

std::string get_string_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size)
{
    std::string str = "";
    str.reserve(size);
    std::streampos posBuffer = file.tellg();

    file.seekg(offset, std::ios::beg);

    file.read(str.data(), size);

    file.seekg(posBuffer, std::ios::beg);

    return str;
}

template<typename T>
T *get_array_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size)
{
    T *result = new T[size / sizeof(T)];
    std::streampos posBuffer = file.tellg();

    file.seekg(offset, std::ios::beg);

    file.read(result, size);

    file.seekg(posBuffer, std::ios::beg);

    return result;
}

}