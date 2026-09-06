#include "comp/rendering/Animator.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/Animator.hpp"

#include <cstring>

namespace mtrs::comp
{

Animator::Animator(COMPONENT_ARGS)
{
    prs::Animator animator;
    std::memcpy(&animator, file_data, sizeof(animator));

    prs::set_mtrs_to_var(file_ddata[animator.durations], durations);
    count_frame = animator.count_frame;
    frame_offset = animator.frame_offset;
}

uint32_t Animator::get_prs_size_imp() noexcept
{
    return sizeof(prs::Animator);
}

}