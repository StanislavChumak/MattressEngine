#include "comp/rendering/Animator.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/Animator.hpp"

#include <fstream>

namespace mtrs::comp
{

Animator::Animator(COMPONENT_ARGS)
{
    prs::Animator animator;
    file.read(reinterpret_cast<char*>(&animator), sizeof(animator));

    prs::set_mtrs_to_var(file, durations, DEFERRED_ARGS(animator, durations));
    count_frame = animator.count_frame;
    frame_offset = animator.frame_offset;
}

}