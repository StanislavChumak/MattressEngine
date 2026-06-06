#include "comp/rendering/StateAnimator.hpp"

#include "util/get_from_file_mtrs.hpp"

#include <fstream>

#include "mtrsstruct/dynamic_field.def"
#include "mtrsstruct/comp_struct/StateAnimator.struct"

namespace mtrs::comp
{

StateAnimator::StateAnimator(COMPONENT_ARGS)
{
    StateAnimator_sc state_anim;
    file.read(reinterpret_cast<char*>(&state_anim), sizeof(state_anim));

    size_t count = state_anim.states_size / sizeof(StateAnimator_sc::State);
    states.reserve(count);

    StateAnimator_sc::State *statesBuffer;
    statesBuffer = util::get_array_from_mtrs_file<StateAnimator_sc::State>(file, DYNAMIC_ARGS(state_anim, states));

    StateAnimator_sc::State *iter;
    for(iter = statesBuffer; iter < (statesBuffer + state_anim.states_size); iter++)
    {
        std::string key = util::get_string_from_mtrs_file(file, DYNAMIC_ARGS(*iter, name));
        states.emplace(key, State{iter->offset, iter->count});
        if(current_state == "") current_state = key;
    }
}

void StateAnimator::set_state(std::string state)
{
    current_state = state;
    dirty = true;
}

}