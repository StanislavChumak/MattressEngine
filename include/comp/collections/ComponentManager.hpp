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
    std::vector<EntityID> _destroy_queue;

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

    void remove_entity(EntityID id);

public:
    ComponentManager() = default;
    ComponentManager(ComponentManager &) = delete;
    ComponentManager &operator=(const ComponentManager &) = delete;
    ComponentManager(ComponentManager &&other) noexcept;
    ComponentManager &operator=(ComponentManager &&other) noexcept;
    ~ComponentManager();

    EntityID create_entity(std::string name);
    void mark_destroy(EntityID id);
    void remove_marked();
    EntityID get_name_entity(std::string name);

    void clear_sets();
    void clear_singletons();

    template<typename Component> SparseSet<Component>& get_set();
    template<typename Component> void clear_set();
    template<typename Component> bool has_set();

    template<typename Component> Component *get_comp(EntityID entity);
    template<typename Component> Component &add_comp(EntityID entity, Component&& component);
    template<typename Component> void remove_comp(EntityID entity);
    template<typename Component> bool has_comp(EntityID entity);

    template<typename Component> Component *get_single_comp();
    template<typename Component> Component &add_single_comp(Component&& component);
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

template<typename Component>
Component& ComponentManager::add_comp(EntityID entity, Component&& component)
{
    return get_set<Component>().add(entity, std::move(component));
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

template<typename Component>
Component& ComponentManager::add_single_comp(Component&& component)
{
    auto& pool = get_single_pool<Component>();
    pool.component = std::make_unique<Component>(std::move(component));
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
