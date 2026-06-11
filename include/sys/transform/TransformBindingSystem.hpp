#ifndef TRANSFORM_BINDING_SYSTEM_HPP
#define TRANSFORM_BINDING_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class TransformBindingSystem : public System<TransformBindingSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::PRE_UPDATE;}
};

}
#endif
