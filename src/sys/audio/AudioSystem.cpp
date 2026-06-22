#include "sys/audio/AudioSystem.hpp"

#define MINIAUDIO_IMPLEMENTATION

#include "comp/ECSWorld.hpp"
#include "comp/audio/Sound.hpp"
#include "comp/audio/Music.hpp"

namespace mtrs::sys
{

void AudioSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    comp::Audio *audio = world.single_comp<comp::Audio>();
    if(!audio) return;
    for(auto [entity, sound] : world.view<comp::Sound>())
    {
        if(sound->is_play)
        {
            auto ptrSound = sound->get_free_sound();
            if(ptrSound)
                ma_sound_start(ptrSound);
            sound->is_play = false;
        }

        if(sound->is_stop)
        {
            for(auto ptrSound : sound->instances)
                ma_sound_stop(ptrSound);
            sound->is_stop = false;
        }

        if(audio->cache_volume_dirty)
            sound->set_volume(audio->sound_volume);
    }

    for(auto [entity, music] : world.view<comp::Music>())
    {
        if(music->is_play)
            ma_sound_start(music->music);

        if(music->is_stop)
            ma_sound_stop(music->music);

        if(audio->cache_volume_dirty)
            music->setVolume(audio->music_volume);
    }
}

}