#include "comp/audio/SoundPlayer.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Audio.hpp"
#include "res/ResourceManager.hpp"
#include "res/asset/Sound.hpp"

#include "util/func/files/data_mtrs_file.hpp"

#include "dynamic_field.def"
#include "comp_struct/SoundPlayer.struct"

namespace mtrs::comp
{

SoundPlayer::SoundPlayer(COMPONENT_ARGS)
{
    SoundPlayer_sc sound_player;
    file.read(reinterpret_cast<char*>(&sound_player), sizeof(sound_player));

    std::string sound_path;
    util::set_string_from_mtrs_file(file, sound_path, DYNAMIC_ARGS(sound_player, sound));
    sound = resource.get_resource<res::Sound>(scene, sound_path);

    sound->init(world.single_comp<comp::Audio>()->impl);

    sound->set_volume(sound_player.volume);
}

}