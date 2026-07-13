#include "comp/rendering/StateAnimator.hpp"

#include "util/files/data_mtrs_file.hpp"
#include "util/mtrs_message.hpp"
#include "util/hash.hpp"

#include <fstream>
#include <vector>

#include "dynamic_field.def"
#include "comp_struct/StateAnimator.struct"

namespace mtrs::comp
{

StateAnimator::StateAnimator(COMPONENT_ARGS)
{
    StateAnimator_sc state_anim;
    file.read(reinterpret_cast<char*>(&state_anim), sizeof(state_anim));

    size_t count = state_anim.states_size / sizeof(StateAnimator_sc::State);
    states.reserve(count);

    current_state.first = 0;

    std::vector<StateAnimator_sc::State> buffer;
    file::set_array_from_mtrs_file(file, buffer, DYNAMIC_ARGS(state_anim, states));

    for(auto &state : buffer)
    {
        uint32_t id = state.id_state;
        states.emplace(id, State{state.offset, state.count});
        if(current_state.first == 0) current_state.first = id; 
    }
}

void StateAnimator::set_state(std::string state)
{
#ifndef FLAG_RELEASE
    auto iter = states.find(util::hash_string<uint32_t>(state));
    if(iter == states.end())
    {
        util::mtrs_error("Animator does not have a state named: ", state);
        return;
    }
#endif
    current_state.first = util::hash_string<uint32_t>(state);
    current_state.second = state;
    dirty = true;
}

}