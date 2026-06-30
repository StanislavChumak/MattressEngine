#include "comp/collections/ComponentManager.hpp"

#include "util/mtrs_message.hpp"
#include "util/hash.hpp"

namespace mtrs::comp
{

ComponentManager::ComponentManager(ComponentManager &&other) noexcept
{
    _clear_sets = std::move(other._clear_sets);
    _clear_singletons = std::move(_clear_singletons);

    _hashes_entity = std::move(other._hashes_entity);
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

        _hashes_entity = std::move(other._hashes_entity);
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

EntityID ComponentManager::create_entity(uint64_t hash)
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
    _hashes_entity.emplace(hash, id);
    return id;
}

void ComponentManager::remove_entity(uint64_t hash)
{
    uint64_t id = _hashes_entity[hash];
    _hashes_entity.erase(hash);
    for(auto fun : _remove_entity)
    {
        fun(id);
    }
    _freed_ids.push(id);
}

EntityID ComponentManager::get_entity_by_name(std::string name)
{
    auto it = _hashes_entity.find(util::hash_string<uint64_t>(name));
    if(it != _hashes_entity.end())
    {
        return it->second;
    }
    util::mtrs_message(util::TipeMessage::ERROR, "Fatal find Entity to name: ", name);
    return NULL_ENTITY;
}

void ComponentManager::turn_on(uint64_t hash)
{
    _disabled_ids.erase(_hashes_entity[hash]);
}

void ComponentManager::turn_off(uint64_t hash)
{
    _disabled_ids.emplace(_hashes_entity[hash]);
}

bool ComponentManager::is_turn_on(uint64_t hash)
{
    return _disabled_ids.find(_hashes_entity[hash]) == _disabled_ids.end();
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