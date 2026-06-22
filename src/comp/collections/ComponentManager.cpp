#include "comp/collections/ComponentManager.hpp"

#include "util/mtrs_message.hpp"

namespace mtrs::comp
{

ComponentManager::ComponentManager(ComponentManager &&other) noexcept
{
    _clear_sets = std::move(other._clear_sets);
    _clear_singletons = std::move(_clear_singletons);

    _names_entity = std::move(other._names_entity);
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

        _names_entity = std::move(other._names_entity);
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

EntityID ComponentManager::create_entity(std::string name)
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
    _names_entity.emplace(std::move(name), id);
    return id;
}

void ComponentManager::remove_entity(EntityID id)
{
    for(auto fun : _remove_entity)
    {
        fun(id);
    }
    _freed_ids.push(id);
}

EntityID ComponentManager::get_entity_by_name(std::string name)
{
    auto it = _names_entity.find(name);
    if(it != _names_entity.end())
    {
        return it->second;
    }
    util::mtrs_message(util::TipeMessage::ERROR, "Fatal find Entity to name: ", name);
    return NULL_ENTITY;
}

void ComponentManager::turn_on(EntityID id)
{
    _disabled_ids.erase(id);
}

void ComponentManager::turn_off(EntityID id)
{
    _disabled_ids.emplace(id);
}

bool ComponentManager::is_turn_on(EntityID id)
{
    return _disabled_ids.find(id) == _disabled_ids.end();
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