#include "res/ResourceManager.hpp"

#include "util/get_files_from_folder.hpp"
#include "util/hash.hpp"

namespace mtrs::res
{

ResourceManager::ResourceManager(const std::string &executable_path,const std::string &resource_path)
:_executable_path(executable_path), _resource_path(resource_path)
{    
    auto files = util::get_files_from_folder(_resource_path, ".mtrs");
    _resource_packs.reserve(files.size());
    _resource_pack_iters.reserve(files.size());

    std::string name;
    for(auto &file : files)
    {
        name = file.substr(_resource_path.size(), (file.size() - _resource_path.size() - 5));
        _resource_packs.emplace(name, ResourcePack{std::ifstream(), 0});
        _resource_pack_iters.push_back(_resource_packs.find(name));
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
    file.seekg(8, std::ios::beg);

    uint64_t res_type_id = util::hash_string<uint64_t>(res_type_name);
    uint64_t res_id = util::hash_string<uint64_t>(res_name);

    uint64_t id = 0, offset = 0;
    while(!file.eof())
    {
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
        if(id == res_type_id)
        {
            break;
        }
        file.seekg(offset, std::ios::beg);
    }

    int count = (offset - file.tellg()) / (res_type_size + sizeof(id));
    for(int i = 0; i < count; i++)
    {
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        if(id == res_id)
        {
            return;
        }
        file.seekg(res_type_size, std::ios::cur);
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
