#include "res/asset/Sound.hpp"

#include "miniaudio.h"

#include "util/files/data_mtrs_file.hpp"
#include "util/mtrs_message.hpp"

#include <vector>

#include "dynamic_field.def"
#include "res_struct/Sound.struct"

namespace mtrs::res
{

struct Sound::Impl
{
    std::vector<ma_sound> sounds;
};

Sound::Sound(ASSET_ARGS)
{
    Sound_sc sound;
    file.read(reinterpret_cast<char*>(&sound), sizeof(sound));

    file::set_string_from_mtrs_file(file, _path, DYNAMIC_ARGS(sound, path));
    _path = dir_resource + _path;

    _impl = new Sound::Impl();

    _flag = 0;
    _flag |= (sound.flag & 1u) ? MA_SOUND_FLAG_LOOPING : 0;
    _flag |= (sound.flag & 2u) ? MA_SOUND_FLAG_STREAM : 0;
    _flag |= (sound.flag & 4u) ? 0 : MA_SOUND_FLAG_NO_SPATIALIZATION;
    _flag |= (sound.flag & 8u) ? 0 : MA_SOUND_FLAG_NO_PITCH;
    _flag |= (sound.count > 1) ? MA_SOUND_FLAG_DECODE : 0;

    _distance = sound.max_distance;

    _impl->sounds.resize(sound.count);
}

Sound::Sound(Sound &&other) noexcept
{
    _impl = other._impl;
    other._impl = nullptr;
    _path = std::move(other._path);
    _flag = other._flag;
    other._flag = 0;
    _distance = other._distance;
    other._distance = 0;
    _is_init = other._is_init;
    other._is_init = false;
}

Sound &Sound::operator=(Sound &&other) noexcept
{
    if(this != &other)
    {
        _impl = other._impl;
        other._impl = nullptr;
        _path = std::move(other._path);
        _flag = other._flag;
        other._flag = 0;
        _distance = other._distance;
        other._distance = 0;
        _is_init = other._is_init;
        other._is_init = false;
    }
    return *this;
}

Sound::~Sound()
{
    for(auto sound : _impl->sounds)
    {
        ma_sound_uninit(&sound);
    }
    delete _impl;
}

std::string Sound::get_type_name_imp() noexcept
{
    return std::string("sounds");
}

uint32_t Sound::get_type_size_imp() noexcept
{
    return sizeof(Sound_sc);
}

bool Sound::init(void *audio_engine)
{
    if(_is_init) return false;

    ma_engine *engine = static_cast<ma_engine*>(audio_engine);

    for(auto &sound : _impl->sounds)
    {
        ma_result result = ma_sound_init_from_file(engine, _path.c_str(), 
            _flag, nullptr, nullptr, &sound);
#ifndef FLAG_RELEASE
        if(result != MA_SUCCESS)
        {
            util::mtrs_error("Failed init sould to path ", _path, " ", result);
        }
#endif
    }
    return true;
}

void Sound::set_volume(float volume)
{
    for(auto &sound : _impl->sounds)
    {
        ma_sound_set_volume(&sound, volume);
    }
}

void *Sound::get_free_sound()
{
    for(auto &sound : _impl->sounds)
    {
        if(!ma_sound_is_playing(&sound))
        {
            return &sound;
        }
    }

    ma_sound *sound = &_impl->sounds[_robin];
    _robin = (_robin + 1) % _impl->sounds.size();
    return sound;
}

void Sound::start()
{
    auto sound = static_cast<ma_sound*>(get_free_sound());
    ma_sound_seek_to_pcm_frame(sound, 0);
    ma_sound_start(sound);
}

void Sound::play()
{
    for(auto &sound : _impl->sounds)
    {
        if(!ma_sound_is_playing(&sound))
        {
            ma_sound_start(&sound);
        }
    }
}

void Sound::stop()
{
    for(auto &sound : _impl->sounds)
    {
        ma_sound_stop(&sound);
    }
}

}