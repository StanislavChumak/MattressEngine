#ifndef AUDIO_SYSTEM_HPP
#define AUDIO_SYSTEM_HPP

namespace mtrs::comp {
    class ECSWorld;
}

namespace mtrs::sys
{

class AudioSystem
{
public:
    AudioSystem() = default;

    static void update(comp::ECSWorld &world, const double &delta);
};

}

#endif
