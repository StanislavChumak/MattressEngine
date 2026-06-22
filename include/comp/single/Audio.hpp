#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "miniaudio.h"

namespace mtrs::comp
{

struct Audio
{
    ma_engine engine;
    bool initialized = false;

    float sound_scale = 100.f;

    float sound_volume = 1.f;
    float music_volume = 1.f;
    bool cache_volume_dirty = false;

    Audio() = delete;
    ~Audio();
    Audio(const Audio &) = delete;
    Audio &operator=(const Audio&) = delete;
    Audio(Audio&&) = delete;
    Audio &operator=(Audio&&) = delete;

    Audio(void*);
};

}

#endif
