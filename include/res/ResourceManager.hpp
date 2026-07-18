#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "util/mtrs_message.hpp"

#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <fstream>

namespace mtrs::res
{

class ResourceManager
{
private:
    struct ResourcePack
    {
        std::ifstream file;
        size_t resource_count;
    };
    std::string _executable_path;
    std::string _resource_dir;
    std::unordered_map<std::string, ResourcePack> _resource_packs;
    std::vector<decltype(_resource_packs)::const_iterator> _resource_pack_iters;

    std::vector<std::function<void()>> _garbage_collectors;

    void move_to_resource(std::ifstream &file, const std::string &res_name,
        const std::string &res_type_name, uint32_t res_type_size);

    template<typename Resource>
    struct cacheWrapper
    {
        std::unordered_map<std::string, std::weak_ptr<Resource>> cache;
        bool initialization = false;
    };

    template<typename Resource>
    std::unordered_map<std::string, std::weak_ptr<Resource>> &get_cache()
    {
        static cacheWrapper<Resource> wrapper;
        if(!wrapper.initialization)
        {
            _garbage_collectors.push_back([&]()
            {
                for (auto it_res = wrapper.cache.begin(); it_res != wrapper.cache.end(); )
                {
                    if (it_res->second.expired())
                    {
                        size_t pos = it_res->first.find_last_of("\\/");
                        _resource_packs[it_res->first.substr(0, pos)].resource_count--;

                        it_res = wrapper.cache.erase(it_res);
                    }
                    else
                    {
                        it_res++;
                    }
                }
            });
            wrapper.initialization = true;
        }
        return wrapper.cache;
    }

public:
    ResourceManager() = delete;
    ResourceManager(const std::string &executable_path,const std::string &resource_dir);
    ResourceManager(ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&other) noexcept;
    ResourceManager &operator=(ResourceManager &&other) noexcept;
    ~ResourceManager();

    void garbage_collector();

    template<typename Resource>
    std::shared_ptr<Resource> get_resource(std::string pack, std::string resource_name)
    {
        size_t pos = resource_name.find_last_of("\\/");
        if(pos != std::string::npos)
        {
            resource_name = resource_name.substr(pos + 1);
            pack = resource_name.substr(0, pos);
        }
        auto &cache = get_cache<Resource>();
        auto it_res = cache.find(pack + '/' + resource_name);
        if(it_res != cache.end())
        {
            if(auto existing = it_res->second.lock())
            {
                return std::static_pointer_cast<Resource>(existing);
            }
        }

        auto it_pack = _resource_packs.find(pack);
        if(it_pack != _resource_packs.end())
        {
            if(!it_pack->second.file.is_open())
            {
                it_pack->second.file.open(_resource_dir + '/' + pack + ".mtpck", std::ios::binary);
            }
            
            std::string res_type_name = Resource::get_type_name();
            uint32_t res_type_size = Resource::get_type_size();

            move_to_resource(it_pack->second.file, resource_name, res_type_name, res_type_size);
            
            std::shared_ptr<Resource> resource = std::make_shared<Resource>(it_pack->second.file, _resource_dir);

            if(!resource)
            {
                util::mtrs_error("in ResourcePack(", pack, ")");
            }

            it_pack->second.resource_count++;
            cache.emplace(pack + '/' +  resource_name, std::weak_ptr<Resource>(resource));
            return resource;
        }
        mtrs::util::mtrs_error("No package named \"", pack,"\" found");
        return std::shared_ptr<Resource>(nullptr);
    }
};

}

#endif
