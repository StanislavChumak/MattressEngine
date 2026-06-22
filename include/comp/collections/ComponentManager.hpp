#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "SparseSet.hpp"
#include "View.hpp"

#include <functional>
#include <memory>

#include <stack>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace mtrs::comp
{

class ComponentManager
{
    std::vector<std::function<void()>> _clear_sets;
    std::vector<std::function<void()>> _clear_singletons;

    std::vector<std::function<void(EntityID)>> _remove_entity;

    EntityID _entity_index = 0;
    std::stack<EntityID> _freed_ids;
    std::unordered_map<std::string, EntityID> _names_entity;
    std::unordered_set<EntityID> _disabled_ids;

    template<typename Component>
    struct ComponentPool
    {
        SparseSet<Component> sparse_set;
        bool initialized = false;
    };

    template<typename Component>
    ComponentPool<Component>& get_pool()
    {
        static ComponentPool<Component> pool;
        if(!pool.initialized)
        {
            _remove_entity.push_back([&](EntityID id){ pool.sparse_set.remove(id);});
            _clear_sets.push_back([&](){ pool.sparse_set.clear();});
            pool.initialized = true;
        }
        return pool;
    }

    template<typename Component>
    struct SingleComponentPool
    {
        std::unique_ptr<Component> component;
        bool initialized = false;
    };

    template<typename Component>
    SingleComponentPool<Component>& get_single_pool()
    {
        static SingleComponentPool<Component> pool;
        if(!pool.initialized)
        {
            _clear_singletons.push_back([&](){ pool.component.reset();});
        }
        return pool;
    }

    

public:
    ComponentManager() = default;
    ComponentManager(ComponentManager &) = delete;
    ComponentManager &operator=(const ComponentManager &) = delete;
    ComponentManager(ComponentManager &&other) noexcept;
    ComponentManager &operator=(ComponentManager &&other) noexcept;
    ~ComponentManager();

    EntityID create_entity(std::string name);
    EntityID get_entity_by_name(std::string name);
    void remove_entity(EntityID id);

    void turn_on(EntityID id);
    void turn_off(EntityID id);
    bool is_turn_on(EntityID id);

    void clear_sets();
    void clear_singletons();

    template<typename Component> SparseSet<Component>& get_set();
    template<typename Component> void clear_set();
    template<typename Component> bool has_set();

    template<typename Component> Component *get_comp(EntityID entity);
    template<typename Component, typename ...Args> Component &add_comp(EntityID entity, Args&& ...args);
    template<typename Component> void remove_comp(EntityID entity);
    template<typename Component> bool has_comp(EntityID entity);

    template<typename Component> Component *get_single_comp();
    template<typename Component, typename ...Args> Component& add_single_comp(Args&& ...args);
    template<typename Component> void remove_single_comp();
    template<typename Component> bool has_single_comp();

    template<typename... Components>
    View<Components...> view()
    {
        return View<Components...>(_disabled_ids, get_set<Components>()...);
    }
};

template<typename Component>
SparseSet<Component>& ComponentManager::get_set()
{
    return get_pool<Component>().sparse_set;
}

template<typename Component>
void ComponentManager::clear_set()
{
    get_set<Component>().clear();
}

template<typename Component>
bool ComponentManager::has_set()
{
    return get_pool<Component>().initialized;
}

template<typename Component>
Component *ComponentManager::get_comp(EntityID entity)
{
    return get_set<Component>().get(entity);
}

template<typename Component, typename ...Args>
Component& ComponentManager::add_comp(EntityID entity, Args&& ...args)
{
    return get_set<Component>().add(entity, args...);
}

template<typename Component>
void ComponentManager::remove_comp(EntityID entity)
{
    get_set<Component>().remove(entity);
}

template<typename Component>
bool ComponentManager::has_comp(EntityID entity)
{
    return get_set<Component>().has(entity);
}

template<typename Component>
Component *ComponentManager::get_single_comp()
{
    auto& pool = get_single_pool<Component>();
    if (!pool.initialized) {
        return nullptr;
    }
    return pool.component.get();
}

template<typename Component, typename ...Args>
Component& ComponentManager::add_single_comp(Args&& ...args)
{
    auto& pool = get_single_pool<Component>();
    pool.component = std::make_unique<Component>(args...);
    pool.initialized = true;
    return *pool.component;
}

template<typename Component>
void ComponentManager::remove_single_comp()
{
    auto& pool = get_single_pool<Component>();
    if (pool.initialized) {
        pool.component.reset();
        pool.initialized = false;
    }
}

template<typename Component>
bool ComponentManager::has_single_comp()
{
    return get_single_pool<Component>().initialized;
}

}

#endif
