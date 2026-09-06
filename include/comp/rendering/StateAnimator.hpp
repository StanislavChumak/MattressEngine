#ifndef STATE_ANIMATOR_HPP
#define STATE_ANIMATOR_HPP

#include "comp/Component.hpp"

#include <string>
#include <unordered_map>

namespace mtrs::comp
{

struct StateAnimator : public Component<StateAnimator>
{
    struct State
    {
        size_t offset;
        size_t count;
    };
    std::unordered_map<uint32_t, State> states;
    uint32_t current_state;
    bool dirty = true;

    StateAnimator() = delete;
    StateAnimator(COMPONENT_ARGS);

    void set_state(std::string state);

    static constexpr const char *get_type_name_imp() noexcept { return "StateAnimator"; }
    static uint32_t get_prs_size_imp() noexcept;
};

}

#endif
