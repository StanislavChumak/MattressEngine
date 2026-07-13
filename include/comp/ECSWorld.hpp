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
    std::string _current_scene;
    struct Scene
    {
        std::unordered_map<uint64_t, EntityID> local_entities;
        std::ifstream file;
        bool init;
        bool turn_on;
    };
    std::string _scenes_path;
    
    ComponentManager _components;
    std::deque<uint64_t> _destroy_queue;

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

    std::string current_scene();

    void mark_destroy(std::string name);
    void remove_marked();

    template<typename... Components>
    View<Components...> view()
    {
        return _components.view<Components...>();
    }

    template<typename Component>
    Component* single_comp()
    {
        return _components.get_single_comp<Component>();
    }

    template<typename Component, typename ...Args>
    Component* single_comp(Args&& ...args)
    {
        return &_components.add_single_comp<Component>(args...);
    }

    void *single_comp(const char *comp);

    template<typename Component>
    Component *component(EntityID entity)
    {
        return _components.get_comp<Component>(entity);
    }

    void *component(const char *comp, EntityID entity);

    EntityID get_entity(const char *scene, uint64_t hash_entity);

    void clear_all();
};

}

#endif
