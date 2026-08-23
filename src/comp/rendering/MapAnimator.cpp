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

    std::vector<prs::MapAnimator::Range> ranges;
    prs::set_mtrs_to_var(file, ranges, DEFERRED_ARGS(map_animator, ranges));
    cell_animators.resize(ranges.size());
    auto iter = cell_animators.begin();
    for(auto &range : ranges)
    {
        iter->count_frame = range.size;
        iter->offset = range.offset;
        iter++;
    }
    
    prs::set_mtrs_to_var(file, durations, DEFERRED_ARGS(map_animator, durations));
}

}