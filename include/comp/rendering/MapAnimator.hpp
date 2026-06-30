#ifndef MAP_ANIMATOR_HPP
#define MAP_ANIMATOR_HPP

#include "comp/Component.hpp"

#include <vector>

namespace mtrs::comp
{

struct MapAnimator
{
    std::vector<float> durations;

    struct CellAnimator
    {
        size_t count_frame = 0;
        size_t offset = 0;

        size_t current_frame = 0;
        float current_animation_time = 0;
    };
    std::vector<CellAnimator> cell_animators;

    MapAnimator() = delete;
    MapAnimator(COMPONENT_ARGS);
};

}

#endif
