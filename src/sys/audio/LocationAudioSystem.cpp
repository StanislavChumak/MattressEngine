#include "sys/audio/LocationAudioSystem.hpp"

#include "comp/ECSWorld.hpp"

#include "comp/core/Transform.hpp"

#include "comp/single/Listener.hpp"
#include "comp/audio/Sound.hpp"
#include "comp/audio/Music.hpp"

namespace mtrs::sys
{

void LocationAudioSystem::update(comp::ECSWorld &world, const double &delta)
{
    comp::Listener *listener = world.get_single_comp<comp::Listener>();
    comp::Audio *audio = world.get_single_comp<comp::Audio>();
    if(!listener || !audio || (listener->target != NULL_ENTITY)) return;
    glm::vec2 listenerPosition = world.get_comp<comp::Transform>(listener->target)->position / audio->sound_scale;
    ma_engine_listener_set_position(&audio->engine, 0, listenerPosition.x, listenerPosition.y, 0.f);
    for(auto [entity, sound, transform] : world.view<comp::Sound, comp::Transform>())
    {
        if(sound->is_location)
        {
            for(auto ptrSound : sound->instances)
                ma_sound_set_position(ptrSound, transform->global_matrix[3].x / audio->sound_scale
                                              , transform->global_matrix[3].y / audio->sound_scale
                                              , transform->global_matrix[3].z / audio->sound_scale);

        }
    }
    for(auto [entity, music, transform] : world.view<comp::Music, comp::Transform>())
    {
        if(music->is_location)
            ma_sound_set_position(music->music, transform->global_matrix[3].x * audio->sound_scale
                                              , transform->global_matrix[3].y * audio->sound_scale
                                              , transform->global_matrix[3].z * audio->sound_scale);
    }
}

}