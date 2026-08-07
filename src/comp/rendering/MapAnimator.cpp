#include "comp/rendering/MapAnimator.hpp"

#include "util/func/files/data_mtrs_file.hpp"

#include <fstream>

#include "dynamic_field.def"
#include "comp_struct/MapAnimator.struct"

namespace mtrs::comp
{

MapAnimator::MapAnimator(COMPONENT_ARGS)
{
    MapAnimator_sc map_animator;
    file.read(reinterpret_cast<char*>(&map_animator), sizeof(map_animator));

    std::vector<MapAnimator_sc::Range> ranges;
    util::set_array_from_mtrs_file(file, ranges, DYNAMIC_ARGS(map_animator, ranges));
    cell_animators.resize(ranges.size());
    auto iter = cell_animators.begin();
    for(auto &range : ranges)
    {
        iter->count_frame = range.size;
        iter->offset = range.offset;
        iter++;
    }
    
    util::set_array_from_mtrs_file(file, durations, DYNAMIC_ARGS(map_animator, durations));
}

}