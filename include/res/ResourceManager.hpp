#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "util/func/mtrs_message.hpp"

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

    bool move_to_resource(std::ifstream &file, const std::string &res_name,
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
                        size_t str_pos = it_res->first.find_last_of("\\/");
                        _resource_packs[it_res->first.substr(0, str_pos)].resource_count--;

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
        size_t str_pos = resource_name.find_last_of("\\/");
        if(str_pos != std::string::npos)
        {
            pack = resource_name.substr(0, str_pos);
            resource_name = resource_name.substr(str_pos + 1);
        }
        if(pack.find(_resource_dir) == 0) pack = pack.substr(_resource_dir.length());
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
            bool file_is_open = it_pack->second.file.is_open();
            std::streampos file_pos;
            if(file_is_open)
            {
                file_pos = it_pack->second.file.tellg();
            }
            else
            {
                it_pack->second.file.open(_resource_dir + '/' + pack + ".mtpck", std::ios::binary);
            }
            
            std::string res_type_name = Resource::get_type_name();
            uint32_t res_type_size = Resource::get_type_size();

            if(!move_to_resource(it_pack->second.file, resource_name, res_type_name, res_type_size))
            {
#ifndef FLAG_RELEASE
                util::mtrs_error("from resource pack (", pack, ")");
                return std::shared_ptr<Resource>(nullptr);
#endif
            }
            
            std::shared_ptr<Resource> resource;
            str_pos = pack.find_last_of("\\/") + 1;
            resource = std::make_shared<Resource>(_resource_dir + pack.substr(0, str_pos),
                pack.substr(str_pos), it_pack->second.file, *this);

            if(file_is_open)
            {
                it_pack->second.file.seekg(file_pos);
            }
            else
            {
                it_pack->second.file.close();
            }

#ifndef FLAG_RELEASE
            if(!resource)
            {
                util::mtrs_error("failed to initialize resource (", res_type_name, ": ",
                    resource_name, ") from resource pack (", pack, ")");
                return std::shared_ptr<Resource>(nullptr);
            }
#endif
            it_pack->second.resource_count++;
            cache.emplace(pack + '/' +  resource_name, std::weak_ptr<Resource>(resource));
            return resource;
        }
#ifndef FLAG_RELEASE
        if(pack != "null") mtrs::util::mtrs_error("No package named \"", pack,"\" found");
#endif
        return std::shared_ptr<Resource>(nullptr);
    }
};

}

#endif
