#include "sys/rendering/AnimatorSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/Animator.hpp"

namespace mtrs::sys
{

void AnimatorSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, sprite, animator] : world.view<comp::Sprite, comp::Animator>())
    {
        animator->current_animation_time += delta;

        while (animator->current_animation_time >= animator->durations[animator->current_frame])
        {
            animator->current_animation_time -= animator->durations[animator->current_frame];
            animator->current_frame++;

            if (animator->current_frame == animator->count_frame + animator->offset)
            {
                animator->current_frame = animator->offset;
            }

            sprite->sub_texture = sprite->atlas->get_sub_texture(animator->current_frame);
        }
    }
}

}