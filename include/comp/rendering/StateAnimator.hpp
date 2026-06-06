#ifndef STATE_ANIMATOR_HPP
#define STATE_ANIMATOR_HPP

#include "comp/Component.hpp"

#include <string>
#include <unordered_map>

namespace mtrs::comp
{

struct StateAnimator
{
    struct State
    {
        size_t offset;
        size_t count;
    };
    std::unordered_map<std::string, State> states;
    std::string current_state = "";
    bool dirty = true;

    StateAnimator() = delete;
    StateAnimator(COMPONENT_ARGS);

    void set_state(std::string state);
};

}

#endif
