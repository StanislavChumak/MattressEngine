#include "util/files/get_folder.hpp"

#include "util/mtrs_message.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace mtrs::file
{

std::unordered_set<std::string> get_files_from_folder(const std::string &dirPath, const std::string &extension)
{
    std::unordered_set<std::string> files;
    try
    {
        for (const auto& entry : fs::directory_iterator(dirPath))
        {
            fs::path p = entry.path();
            if (entry.is_directory())
            {
                files.merge(get_files_from_folder(p, extension));
            }
            else if (entry.is_regular_file() && p.extension() == extension)
            {
                files.emplace(p);
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
#ifndef FLAG_RELEASE
        util::mtrs_error(e.what());
#endif
    }
    return files;
}

}