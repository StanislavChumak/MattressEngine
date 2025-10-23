#define MINIAUDIO_IMPLEMENTATION
#include "../../external/miniaudio.h"

#include "Sound.h"

#include "../system/String.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif


ma_engine Sound::_maEngine;

bool Sound::init_engine()
{
    return ma_engine_init(NULL, &_maEngine) != MA_SUCCESS;
}

void Sound::uninit_engine()
{
    ma_engine_uninit(&_maEngine);
}

Sound::Sound(sys::Str path, ma_uint32 flag)
: _thisSound(new ma_sound)
{
    if(ma_sound_init_from_file(&_maEngine, path.c_str(), flag, nullptr, nullptr, _thisSound) != MA_SUCCESS)
    {
#ifndef FLAG_RELEASE
        std::cerr << "Failed init sould to path " << path.c_str() << std::endl;
#endif
    }
}

Sound::~Sound()
{
    ma_sound_uninit(_thisSound);
}

void Sound::play()
{
    ma_sound_start(_thisSound);
}

void Sound::stop()
{
    ma_sound_stop(_thisSound);
}

void Sound::set_volume(float volume)
{
    ma_sound_set_volume(_thisSound, volume);
}

void Sound::set_position(float x, float y, float z)
{
    ma_sound_set_position(_thisSound, x, y, z);
}

bool Sound::is_playing() const
{
    return ma_sound_is_playing(_thisSound);
}