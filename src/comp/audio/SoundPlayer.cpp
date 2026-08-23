#include "comp/audio/SoundPlayer.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Audio.hpp"
#include "res/ResourceManager.hpp"
#include "res/asset/Sound.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/SoundPlayer.hpp"

namespace mtrs::comp
{

SoundPlayer::SoundPlayer(COMPONENT_ARGS)
{
    prs::SoundPlayer sound_player;
    file.read(reinterpret_cast<char*>(&sound_player), sizeof(sound_player));

    std::string sound_path;
    prs::set_mtrs_to_var(file, sound_path, DEFERRED_ARGS(sound_player, sound));
    sound = resource.get_resource<res::Sound>(scene, sound_path);

    sound->init(world.single_comp<comp::Audio>()->impl);

    sound->set_volume(sound_player.volume);
}

}