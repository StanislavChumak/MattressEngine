#include "sys/audio/LocationAudioSystem.hpp"

#include "comp/ECSWorld.hpp"

#include "comp/core/Transform.hpp"

#include "comp/single/Listener.hpp"
#include "comp/audio/Sound.hpp"
#include "comp/audio/Music.hpp"

namespace mtrs::sys
{

void LocationAudioSystem::update_imp(comp::ECSWorld &world, const double &delta)
{
    // comp::Listener *listener = world.single_comp<comp::Listener>();
    // comp::Audio *audio = world.single_comp<comp::Audio>();
    // if(!listener || !audio || (listener->target != NULL_ENTITY)) return;
    // glm::vec2 listenerPosition = world.component_manager().get_comp<comp::Transform>(listener->target)->position.get() / audio->sound_scale;
    // ma_engine_listener_set_position(&audio->engine, 0, listenerPosition.x, listenerPosition.y, 0.f);
    // for(auto [entity, sound, transform] : world.view<comp::Sound, comp::Transform>())
    // {
    //     auto m = transform->matrix.get();
    //     if(sound->is_location)
    //     {
    //         for(auto ptr_sound : sound->instances)
    //         {
    //             ma_sound_set_position(ptr_sound, m[3].x / audio->sound_scale, m[3].y / audio->sound_scale, 0);
    //         }

    //     }
    // }
    // for(auto [entity, music, transform] : world.view<comp::Music, comp::Transform>())
    // {
    //     auto m = transform->matrix.get();
    //     if(music->is_location)
    //     {
    //         ma_sound_set_position(music->music,
    //             m[3].x * audio->sound_scale, m[3].y * audio->sound_scale, 0);
    //     }
            
    // }
}

}