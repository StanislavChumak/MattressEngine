#include "sys/SystemManager.hpp"

#include <algorithm>

namespace mtrs::sys
{

SystemManager::SystemManager()
: _cache_dirty(true), _cache_state("")
{}

void SystemManager::rebuild_cache()
{
    _cached_merged_updates = _always_updates;
    _cached_merged_updates.reserve(_always_updates.size() + _state_update_map[_cache_state].size());

    _cached_merged_updates.insert(_cached_merged_updates.end(),
        _state_update_map[_cache_state].begin(), _state_update_map[_cache_state].end());

    std::sort(_cached_merged_updates.begin(), _cached_merged_updates.end(),
        [&](std::type_index a, std::type_index b) { return _updates[a].first > _updates[b].first; });
}

void SystemManager::update(comp::ECSWorld &world, const double &delta, const std::string &currentState)
{
    if(_cache_dirty || _cache_state != currentState)
    {
        _cache_state = currentState;
        rebuild_cache();
    }

    for(auto id : _cached_merged_updates)
    {
        _updates[id].second(world, delta);
    }
}

void SystemManager::set_state_updates(std::string state , std::vector<std::type_index> &&systems)
{
    _state_update_map.try_emplace(std::move(state), std::move(systems));
}

void SystemManager::add_state_update(std::string state , std::type_index system)
{
    _state_update_map[state].push_back(system);
}

}