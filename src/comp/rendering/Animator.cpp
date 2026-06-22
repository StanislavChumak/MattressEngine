#include "comp/rendering/Animator.hpp"

#include "util/get_from_file_mtrs.hpp"

#include <fstream>

#include "mtrsstruct/dynamic_field.def"
#include "mtrsstruct/comp_struct/Animator.struct"

namespace mtrs::comp
{

Animator::Animator(COMPONENT_ARGS)
{
    Animator_sc animator;
    file.read(reinterpret_cast<char*>(&animator), sizeof(animator));

    durations = util::get_array_from_mtrs_file<float>(file, DYNAMIC_ARGS(animator, durations));
    count_frame = animator.durations_size / sizeof(float);
}

Animator::Animator(Animator&& other) noexcept
{
    count_frame = other.count_frame;
    other.count_frame = 0;
    durations = other.durations;
    other.durations = nullptr;
    current_frame = other.count_frame;
    other.count_frame = 0;
    current_animation_time = other.current_animation_time;
    other.current_animation_time = 0;
    offset = other.offset;
    other.offset = 0;
}

Animator &Animator::operator=(Animator&& other) noexcept
{
    if(this != &other)
    {
        count_frame = other.count_frame;
        other.count_frame = 0;
        durations = other.durations;
        other.durations = nullptr;
        current_frame = other.count_frame;
        other.count_frame = 0;
        current_animation_time = other.current_animation_time;
        other.current_animation_time = 0;
        offset = other.offset;
        other.offset = 0;
    }
    return *this;
}

Animator::~Animator()
{
    delete[] durations;
}

}