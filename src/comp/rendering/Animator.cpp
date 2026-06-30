#include "comp/rendering/Animator.hpp"

#include "util/set_from_file_mtrs.hpp"

#include <fstream>

#include "dynamic_field.def"
#include "comp_struct/Animator.struct"

namespace mtrs::comp
{

Animator::Animator(COMPONENT_ARGS)
{
    Animator_sc animator;
    file.read(reinterpret_cast<char*>(&animator), sizeof(animator));

    util::set_array_from_mtrs_file(file, durations, DYNAMIC_ARGS(animator, durations));
    count_frame = durations.size();
}

}