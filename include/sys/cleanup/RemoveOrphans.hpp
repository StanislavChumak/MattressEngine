#ifndef REMOVE_ORPHANS_HPP
#define REMOVE_ORPHANS_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class RemoveOrphans
{
public:
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
