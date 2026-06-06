#include "sys/rendering/StateAnimatorSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/rendering/Animator.hpp"
#include "comp/rendering/StateAnimator.hpp"

namespace mtrs::sys
{

void StateAnimatorSystem::update(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, animator, state] : world.view<comp::Animator, comp::StateAnimator>())
    {
        if(!state->dirty) continue;

        auto it = state->states.find(state->current_state);
        if(it != state->states.end())
        {
            animator->current_frame = animator->offset = it->second.offset;
            animator->count_frame = it->second.count;
            animator->current_animation_time = 0;
        }
        state->dirty = false;
    }
}

}