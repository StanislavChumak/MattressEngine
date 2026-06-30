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
    std::unordered_map<uint32_t, State> states;
    std::pair<uint32_t, std::string> current_state;
    bool dirty = true;

    StateAnimator() = delete;
    StateAnimator(COMPONENT_ARGS);

    void set_state(std::string state);
};

}

#endif
