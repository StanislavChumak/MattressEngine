#include "sys/rendering/MapAnimatorSystem.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/rendering/SpriteMap.hpp"
#include "comp/rendering/MapAnimator.hpp"
#include "res/asset/TextureAtlas.hpp"

namespace mtrs::sys
{

void MapAnimatorSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    for(auto [entity, map, map_animator] : world.view<comp::SpriteMap, comp::MapAnimator>())
    {
        for(int i = 0; i < map_animator->cell_animators.size(); i++)
        {
            auto &animator = map_animator->cell_animators[i];
            animator.current_animation_time += delta;

            while (animator.current_animation_time >= map_animator->durations[animator.current_frame])
            {
                animator.current_animation_time -= map_animator->durations[animator.current_frame];
                animator.current_frame++;

                if (animator.current_frame == animator.count_frame + animator.offset)
                {
                    animator.current_frame = animator.offset;
                }

                map->cell_types[i] = map->atlas->get_sub_texture(animator.current_frame);
            }
        }
    }
}

}