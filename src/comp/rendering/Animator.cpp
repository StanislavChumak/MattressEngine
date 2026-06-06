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
    count_frame = sizeof(durations) / sizeof(*durations);
}

Animator::~Animator()
{
    delete[] durations;
}

}