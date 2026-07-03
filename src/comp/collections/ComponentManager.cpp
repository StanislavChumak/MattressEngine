#include "comp/collections/ComponentManager.hpp"

#include "util/mtrs_message.hpp"
#include "util/hash.hpp"

namespace mtrs::comp
{

ComponentManager::ComponentManager(ComponentManager &&other) noexcept
{
    _clear_sets = std::move(other._clear_sets);
    _clear_singletons = std::move(_clear_singletons);

    _remove_entity = std::move(other._remove_entity);
    

    _entity_index = other._entity_index;
    other._entity_index = 0;
    _freed_ids = std::move(other._freed_ids);
}

ComponentManager &ComponentManager::operator=(ComponentManager &&other) noexcept
{
    if(this != &other)
    {
        _clear_sets = std::move(other._clear_sets);
        _clear_singletons = std::move(_clear_singletons);

        _remove_entity = std::move(other._remove_entity);

        _entity_index = other._entity_index;
        other._entity_index = 0;
        _freed_ids = std::move(other._freed_ids);
    }
    return *this;
}

ComponentManager::~ComponentManager()
{
}

EntityID ComponentManager::create_entity()
{
    EntityID id;
    if(_freed_ids.empty())
    {
        id = _entity_index;
        _entity_index++;
    }
    else
    {
        id = _freed_ids.top();
        _freed_ids.pop();
    }
    return id;
}

void ComponentManager::remove_entity(EntityID entity)
{
    for(auto fun : _remove_entity)
    {
        fun(entity);
    }
    _freed_ids.push(entity);
}

void ComponentManager::turn_on(EntityID entity)
{
    _disabled_ids.erase(entity);
}

void ComponentManager::turn_off(EntityID entity)
{
    _disabled_ids.emplace(entity);
}

void ComponentManager::reserve_turn_off(size_t count)
{
    _disabled_ids.reserve(count + _disabled_ids.size());
}

bool ComponentManager::is_turn_on(EntityID entity)
{
    return _disabled_ids.find(entity) == _disabled_ids.end();
}

void ComponentManager::clear_sets()
{
    for(auto fun : _clear_sets)
    {
        fun();
    }
}

void ComponentManager::clear_singletons()
{
    for(auto fun : _clear_singletons)
    {
        fun();
    }
}

}