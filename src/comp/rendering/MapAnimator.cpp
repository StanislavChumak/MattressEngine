#include "comp/rendering/MapAnimator.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/MapAnimator.hpp"

#include <fstream>

namespace mtrs::comp
{

MapAnimator::MapAnimator(COMPONENT_ARGS)
{
    prs::MapAnimator map_animator;
    file.read(reinterpret_cast<char*>(&map_animator), sizeof(map_animator));

    std::vector<prs::MapAnimator::CellAnimator> cell_animators;
    prs::set_mtrs_to_var(file, cell_animators, DEFERRED_ARGS(map_animator, cell_animators));
    this->cell_animators.resize(cell_animators.size());
    auto iter = this->cell_animators.begin();
    for(auto &anim : cell_animators)
    {
        iter->count_frame = anim.count_frame;
        iter->frame_offset = anim.frame_offset;
        iter++;
    }
    
    prs::set_mtrs_to_var(file, durations, DEFERRED_ARGS(map_animator, durations));
}

}