#ifndef AUDIO_SYSTEM_HPP
#define AUDIO_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class AudioSystem : public System<AudioSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::AUDIO; };
};

}

#endif
