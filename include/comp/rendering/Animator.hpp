#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "comp/Component.hpp"

namespace mtrs::comp
{

struct Animator {
    size_t count_frame = 0;
    float *durations;

    size_t current_frame = 0;
    float current_animation_time = 0;
    size_t offset = 0;

    Animator() = delete;
    Animator(COMPONENT_ARGS);
    Animator(const Animator&) = delete;
    Animator &operator=(const Animator&) = delete;
    Animator(Animator&&) noexcept;
    Animator &operator=(Animator&&) noexcept;
    ~Animator();
};

}

#endif
