#include "comp/rendering/StateAnimator.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_message.hpp"
#include "util/type/prs/comp/StateAnimator.hpp"

#include <fstream>
#include <vector>

namespace mtrs::comp
{

StateAnimator::StateAnimator(COMPONENT_ARGS)
{
    prs::StateAnimator state_anim;
    file.read(reinterpret_cast<char*>(&state_anim), sizeof(state_anim));

    size_t count = state_anim.states_size / sizeof(prs::StateAnimator::State);
    states.reserve(count);

    current_state.first = 0;

    std::vector<prs::StateAnimator::State> buffer;
    prs::set_mtrs_to_var(file, buffer, DEFERRED_ARGS(state_anim, states));

    for(auto &state : buffer)
    {
        uint32_t id = state.id;
        states.emplace(id, State{state.offset, state.count});
        if(current_state.first == 0) current_state.first = id; 
    }
}

void StateAnimator::set_state(std::string state)
{
#ifndef FLAG_RELEASE
    auto iter = states.find(math::hash32(state));
    if(iter == states.end())
    {
        msg::mtrs_error("Animator does not have a state named: ", state);
        return;
    }
#endif
    current_state.first = math::hash32(state);
    current_state.second = state;
    dirty = true;
}

}