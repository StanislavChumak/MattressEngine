#include "comp/single/Audio.hpp"

namespace mtrs::comp
{

Audio::Audio(void*)
{
    ma_engine_config config = ma_engine_config_init();
    config.listenerCount = 1;
    if (ma_engine_init(&config, &engine) == MA_SUCCESS)
    {
        initialized = true;
    }
}

Audio::~Audio()
{
    if (initialized)
    {
        ma_engine_stop(&engine);
        ma_engine_uninit(&engine);
        initialized = false;
    }
}

}