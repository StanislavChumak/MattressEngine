#include "comp/rendering/MapAnimator.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/MapAnimator.hpp"

#include <cstring>

namespace mtrs::comp
{

MapAnimator::MapAnimator(COMPONENT_ARGS)
{
    prs::MapAnimator map_animator;
    std::memcpy(&map_animator, file_data, sizeof(map_animator));

    std::vector<prs::MapAnimator::CellAnimator> cell_animators;
    prs::set_mtrs_to_var(file_ddata[map_animator.cell_animators], cell_animators);
    this->cell_animators.resize(cell_animators.size());
    auto iter = this->cell_animators.begin();
    for(auto &anim : cell_animators)
    {
        iter->count_frame = anim.count_frame;
        iter->frame_offset = anim.frame_offset;
        iter++;
    }
    
    prs::set_mtrs_to_var(file_ddata[map_animator.durations], durations);
}

uint32_t MapAnimator::get_prs_size_imp() noexcept
{
    return sizeof(prs::MapAnimator);
}

}