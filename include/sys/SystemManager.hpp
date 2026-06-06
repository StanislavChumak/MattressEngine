#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <typeindex>

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

namespace SystemPriority
{
    constexpr int INPUT = 10;
    constexpr int NETWORK = 20;
    constexpr int GAMEPLAY_EARLY = 30;
    constexpr int AI_PLANNING = 40;
    constexpr int PLAYER_CONTROL = 50;
    constexpr int AI_EXECUTION = 60;
    constexpr int PHYSICS = 70;
    constexpr int TRANSFORM = 80;
    constexpr int COLLISION = 90;
    constexpr int ANIMATION = 100;
    constexpr int AUDIO = 110;
    constexpr int UI_LOGIC = 120;
    constexpr int RENDERING = 200;
    constexpr int CLEANUP = 1000;
}

class SystemManager
{
private:
    std::unordered_set<std::string> _states;
    std::unordered_map<std::type_index, std::pair<int, void(*)(comp::ECSWorld&, const double&)>> _updates;
    std::unordered_map<std::string, std::vector<std::type_index>> _state_update_map;
    std::vector<std::type_index> _always_updates; 

    void rebuild_cache();
    std::vector<std::type_index> _cached_merged_updates;
    bool _cache_dirty;
    std::string _cache_state;
public:
    SystemManager();

    void update(comp::ECSWorld &world, const double &delta, const std::string &currentState);

    bool add_state(std::string state);

    template <typename T>
    void register_updete(int priority)
    {
        _updates.try_emplace(std::type_index(typeid(T)), std::make_pair<int, void(*)(comp::ECSWorld&, const double&)>(std::move(priority), T::update));
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
