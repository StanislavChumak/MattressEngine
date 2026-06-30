#include "util/set_from_file_mtrs.hpp"

namespace mtrs::util
{

std::string get_string_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size)
{
    char* str = new char[size+1];
    str[size] = '\0';

    std::streampos buffer = file.tellg();
    
    file.seekg(offset, std::ios::beg);
    
    file.read(str, size);
    
    file.seekg(buffer, std::ios::beg);
    
    return static_cast<const char*>(str);
}

void set_string_from_mtrs_file(std::ifstream &file, std::string &dest, uint32_t offset, uint32_t size)
{
    std::streampos buffer = file.tellg();
    
    file.seekg(offset, std::ios::beg);

    dest.resize(size);
    file.read(dest.data(), size);
    
    file.seekg(buffer, std::ios::beg);
}

}