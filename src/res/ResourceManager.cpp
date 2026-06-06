#include "res/ResourceManager.hpp"

#include "util/get_files_from_folder.hpp"

namespace mtrs::res
{

ResourceManager::ResourceManager(const std::string &executablePath,const std::string &resourcePath)
:_executable_path(executablePath)
{
    auto files = util::get_files_from_folder(resourcePath, ".mtrs");
    _resource_packs.reserve(files.size());
    _resource_pack_iters.reserve(files.size());
    for(auto &path : files)
    {
        _resource_packs.emplace(path, ResourcePack{std::ifstream(), 0});
        _resource_pack_iters.push_back(_resource_packs.find(path));
    }
}

ResourceManager::ResourceManager(ResourceManager &&other) noexcept
{
    _executable_path = std::move(other._executable_path);
    _resource_packs = std::move(other._resource_packs);
    _resource_pack_iters = std::move(other._resource_pack_iters);
    _garbage_collectors = std::move(other._garbage_collectors);
}

ResourceManager &ResourceManager::operator=(ResourceManager &&other) noexcept
{
    if(this != &other)
    {
        _executable_path = std::move(other._executable_path);
        _resource_packs = std::move(other._resource_packs);
        _resource_pack_iters = std::move(other._resource_pack_iters);
        _garbage_collectors = std::move(other._garbage_collectors);
    }
    return *this;
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::move_to_resource(std::ifstream &file, const std::string &res_name,
        const std::string &res_type_name, uint32_t res_type_size)
{
    file.seekg(16, std::ios::beg);

    char *name;
    uint32_t size, offset, block_size;
    std::streampos cursor_buffer;
    while(!file.eof())
    {
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        name = new char[size];
        file.read(name, size);
        file.read(reinterpret_cast<char*>(&block_size), sizeof(block_size));
        if(name == res_type_name)
            break;
        file.seekg(block_size, std::ios::cur);
    }
    delete(name);

    int count = block_size / res_type_size;
    for(int i=0; i < count; i++)
    {
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        if(res_name.size() == size)
        {
            name = new char[size];
            file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
            cursor_buffer = file.tellg();
            file.seekg(offset, std::ios::beg);
            file.read(name, size);
            if(name == res_name)
            {
                return;
            }
            delete(name);
        }
        file.seekg(res_type_size - sizeof(size), std::ios::cur);
    }

    util::mtrs_message(util::TipeMessage::ERROR, "Failed to get resource(",
        res_type_name, ": ", res_name,") from resource pack");
    file.close();
}

void ResourceManager::garbage_collector()
{
    for(auto &collector : _garbage_collectors)
    {
        collector();
    }

    for(auto &iter : _resource_pack_iters)
    {
        if(iter->second.resource_count == 0 && iter->second.file.is_open())
        {
            _resource_packs[iter->first].file.close();
        }
    }
    
}

}
