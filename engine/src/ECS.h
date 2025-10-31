#ifndef ECS_H
#define ECS_H

#include "SparseSet.h"
#include "View.h"
#include <new>
#include <type_traits>

#include <stdexcept>

class ECSWorld
{
private:

    template<typename Component>
    struct ComponentPool {
        SparseSet<Component> sparseSet;
        bool initialized = false;
    };

    template<typename Component>
    static inline ComponentPool<Component>& getPool()
    {
        static ComponentPool<Component> pool;
        return pool;
    }

    template<typename Component>
    struct SingleComponentPool {
        alignas(Component) std::byte storage[sizeof(Component)];
        bool initialized = false;
        
        ~SingleComponentPool() {
            if (initialized) {
                get()->~Component();
            }
        }
        
        Component* get() { 
            return std::launder(reinterpret_cast<Component*>(storage));
        }
        
        const Component* get() const { 
            return std::launder(reinterpret_cast<const Component*>(storage));
        }
    };

    template<typename Component>
    static inline SingleComponentPool<Component>& getSinglePool()
    {
        static SingleComponentPool<Component> pool;
        return pool;
    }

    EntityID nextEntity = 0;
    
public:

    EntityID createEntity() { return nextEntity++; }

    template<typename Component>
    SparseSet<Component>& getComponentStorage()
    {
        auto& pool = getPool<Component>();
        if (!pool.initialized)
        {
            pool.sparseSet = SparseSet<Component>();
            pool.initialized = true;
        }
        return pool.sparseSet;
    }
    
    template<typename Component>
    Component* getComponent(EntityID entity) {
        return getComponentStorage<Component>().get(entity);
    }

    template<typename Component>
    void addComponent(EntityID entity, Component component)
    {
        getComponentStorage<Component>().add(entity, std::move(component));
    }

    template<typename Component>
    void removeComponent(EntityID entity)
    {
        getComponentStorage<Component>().remove(entity);
    }

    template<typename Component>
    bool hasComponent()
    {
        return getPool<Component>().initialized;
    }

    //-----------------------------

    template<typename Component>
    Component& getSingleComponent()
    {
        auto& pool = getSinglePool<Component>();
        if (!pool.initialized) {
            throw std::runtime_error("Single component not initialized");
        }
        return *pool.get();
    }

    template<typename Component>
    Component& addSingleComponent(Component component)
    {
        auto& pool = getSinglePool<Component>();
        if (pool.initialized) {
            pool.get()->~Component();
        }
        new (pool.storage) Component(std::move(component));
        pool.initialized = true;
        return *pool.get();
    }

    template<typename Component>
    bool hasSingleComponent()
    {
        return getSinglePool<Component>().initialized;
    }

    template<typename Component>
    void removeSingleComponent()
    {
        auto& pool = getSinglePool<Component>();
        if (pool.initialized) {
            pool.get()->~Component();
            pool.initialized = false;
        }
    }

    //------------------------------

    template<typename... ComponentTypes>
    View<ComponentTypes...> view() {
        return View<ComponentTypes...>(getComponentStorage<ComponentTypes>()...);
    }

    
};

#endif
