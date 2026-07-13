#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "comp/Component.hpp"

namespace mtrs::comp
{

struct Audio : public Component<Audio>
{
    struct Impl;
    Impl *impl;

    void *ptr_engine();

    bool is_init = false;

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
