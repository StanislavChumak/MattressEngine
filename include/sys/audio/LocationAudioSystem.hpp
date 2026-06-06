#ifndef LOCATION_AUDIO_SYSTEM_HPP
#define LOCATION_AUDIO_SYSTEM_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class LocationAudioSystem
{
public:
    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
