#ifndef SYSTEM_HPP
#define SYSTEM_HPP

namespace mtrs::comp
{
    class ECSWorld;
}

namespace mtrs::sys
{

namespace SystemPriority
{
    constexpr int PRE_UPDATE = 0;
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

template <typename Derived>
class System
{
public:
    static void update(comp::ECSWorld &world, const double &delta)
    {
        Derived::update_imp(world, delta);
    }

    static constexpr const char *get_system_name()
    {
        return Derived::get_system_name_imp();
    }

    static constexpr int get_prioritet()
    {
        return Derived::get_prioritet_imp();
    }
};

}

#endif
