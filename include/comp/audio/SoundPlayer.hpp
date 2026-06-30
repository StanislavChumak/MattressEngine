#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include "comp/single/Audio.hpp"

#include "comp/Component.hpp"

#include <memory>

namespace mtrs::res
{
    class Sound;
}

namespace mtrs::comp
{

struct SoundPlayer
{
    std::shared_ptr<res::Sound> sound;

    SoundPlayer() = delete;
    SoundPlayer(COMPONENT_ARGS);
};

}

#endif
