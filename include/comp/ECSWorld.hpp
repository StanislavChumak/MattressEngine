#ifndef ECSWORLD_HPP
#define ECSWORLD_HPP

#include "collections/View.hpp"
#include "collections/ComponentManager.hpp"

#include <fstream>

#include <unordered_map>
#include <string>

namespace mtrs::res
{
    class ResourceManager;
}

namespace mtrs::comp
{

class ECSWorld
{
    struct Scene
    {
        std::ifstream file;
        bool init;
    };
    
    ComponentManager _components;

    std::string _executable_path;
    std::unordered_map<std::string, Scene> _scenes;

public:
    ECSWorld() = delete;
    ECSWorld(const std::string &executable_path,const std::string &scenes_path);
    ECSWorld(ECSWorld &) = delete;
    ECSWorld &operator=(const ECSWorld &) = delete;
    ECSWorld(ECSWorld &&other) noexcept;
    ECSWorld &operator=(ECSWorld &&other) noexcept;
    ~ECSWorld();

    void load_scene(std::string scene, mtrs::res::ResourceManager& resource, bool is_turn_on = true);
    void remove_scene(std::string scene);
    void turn_on_scene(std::string scene);
    void turn_off_scene(std::string scene);

    ComponentManager &component_manager();

    template<typename... Components>
    View<Components...> view()
    {
        return _components.view<Components...>();
    }
};

}

#endif
