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

    void init()
    {
        ma_engine_config config = ma_engine_config_init();
        config.listenerCount = 1;
        if (ma_engine_init(&config, &engine) == MA_SUCCESS)
            initialized = true;
    }

    void destroy()
    {
        if (initialized)
        {
            ma_engine_stop(&engine);
            ma_engine_uninit(&engine);
            initialized = false;
        }
    }

    ~Audio()
    {
        destroy();
    }
};

}

#endif
