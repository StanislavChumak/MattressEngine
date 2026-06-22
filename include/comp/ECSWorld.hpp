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
    std::string _scenes_path;
    
    ComponentManager _components;
    std::deque<EntityID> _destroy_queue;

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

    void mark_destroy(EntityID id);
    void remove_marked();

    template<typename... Components>
    View<Components...> view()
    {
        return _components.view<Components...>();
    }

    template<typename Component, typename ...Args>
    Component* single_comp(Args&& ...args)
    {
        if constexpr (sizeof...(Args) > 0)
        {
            return &_components.add_single_comp<Component>(args...);
        }
        else
        {
            return _components.get_single_comp<Component>();
        }
    }

    void clear_all();
};

}

#endif
