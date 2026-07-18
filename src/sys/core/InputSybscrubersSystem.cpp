#include "sys/core/InputSybscrubersSystem.hpp"


namespace mtrs::sys
{

std::vector<void(*)()> *InputSybscrubersSystem::callbacks;

void InputSybscrubersSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto &callback : *callbacks)
    {
        callback();
    }
    callbacks->clear();
}

}