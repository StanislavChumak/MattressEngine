#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "comp/Component.hpp"

#include <vector>

namespace mtrs::comp
{

struct Animator
{
    size_t count_frame = 0;
    size_t offset = 0;
    
    std::vector<float> durations;

    size_t current_frame = 0;
    float current_animation_time = 0;

    Animator() = delete;
    Animator(COMPONENT_ARGS);
};

}

#endif
