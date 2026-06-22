#include "util/get_from_file_mtrs.hpp"

namespace mtrs::util
{

std::string get_string_from_mtrs_file(std::ifstream &file, uint32_t offset, uint32_t size)
{
    char* str = new char[size+1];
    str[size] = '\0';
    //str.reserve(size+1);
    std::streampos posBuffer = file.tellg();
    
    file.seekg(offset, std::ios::beg);
    
    file.read(str, size);
    
    file.seekg(posBuffer, std::ios::beg);
    
    return static_cast<const char*>(str);
}

}