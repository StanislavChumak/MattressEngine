#ifndef ECSWORLD_HPP
#define ECSWORLD_HPP

#include "collections/View.hpp"
#include "collections/ComponentManager.hpp"


#include <fstream>

#include <unordered_map>
#include <string>

namespace mtrs::comp
{

class ECSWorld
{
    ComponentManager _components;

    std::string _executable_path;
    std::unordered_map<std::string, std::ifstream> _scenes;

public:
    ECSWorld() = delete;
    ECSWorld(const std::string &executable_path,const std::string &scenes_path);
    ECSWorld(ECSWorld &) = delete;
    ECSWorld &operator=(const ECSWorld &) = delete;
    ECSWorld(ECSWorld &&other) noexcept;
    ECSWorld &operator=(ECSWorld &&other) noexcept;
    ~ECSWorld();

    void load_scene(std::string scene);
    void remove_scene(std::string scene);
    void open_scene(std::string scene);
    void close_scene(std::string scene);

    ComponentManager &component_manager();

    template<typename... Components>
    View<Components...> view()
    {
        return _components.view<Components...>();
    }
};

}

#endif
