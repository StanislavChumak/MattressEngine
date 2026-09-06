#include "comp/collections/ComponentManager.hpp"

namespace mtrs::comp
{

ComponentManager::ComponentManager(ComponentManager &&other) noexcept
{
    _entity_index = other._entity_index;
    other._entity_index = 0;
}

ComponentManager &ComponentManager::operator=(ComponentManager &&other) noexcept
{
    if(this != &other)
    {
        _entity_index = other._entity_index;
        other._entity_index = 0;
    }
    return *this;
}

ComponentManager::~ComponentManager()
{
}

EntityID ComponentManager::create_entity()
{
    return _entity_index++;
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

}