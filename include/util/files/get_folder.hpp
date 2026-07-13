#ifndef GET_FOLDER_HPP
#define GET_FOLDER_HPP

#include <unordered_set>
#include <string>

namespace mtrs::file
{

std::unordered_set<std::string> get_files_from_folder(const std::string &dirPath, const std::string &extension);

}

#endif
