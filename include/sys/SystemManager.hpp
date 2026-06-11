#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "System.hpp"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <typeindex>

namespace mtrs::sys
{

class SystemManager
{
private:
    using Update = void(*)(comp::ECSWorld&, const double&);
    std::unordered_map<std::type_index, std::pair<int, Update>> _updates;
    std::unordered_map<std::string, std::vector<std::type_index>> _state_update_map;
    std::vector<std::type_index> _always_updates; 

    void rebuild_cache();
    std::vector<std::type_index> _cached_merged_updates;
    bool _cache_dirty;
    std::string _cache_state;
public:
    SystemManager();

    void update(comp::ECSWorld &world, const double &delta, const std::string &currentState);

    template <typename T>
    void register_update()
    {
        _updates.try_emplace(std::type_index(typeid(T)),
        std::make_pair<int, Update>(T::get_prioritet(), T::update));
    }

    void set_state_updates(std::string state , std::vector<std::type_index> &&systems);
    void add_state_update(std::string state , std::type_index system);

    template <typename T>
    void register_always_update()
    {
        _always_updates.push_back(std::type_index(typeid(T)));
    }
};

}

#endif
