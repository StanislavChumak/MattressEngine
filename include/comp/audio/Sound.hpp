#ifndef SOUND_HPP
#define SOUND_HPP

#include "comp/single/Audio.hpp"

#include "comp/Component.hpp"

namespace mtrs::comp
{

struct Sound
{
    std::vector<ma_sound*> instances;
    size_t count;

    float scale_volume = 1.f;
    bool is_location = false;

    bool is_play = false;
    bool is_stop = false;


    Sound() = default;
    Sound(const Sound&) = delete;
    Sound &operator=(const Sound&) = delete;
    Sound(Sound &&other)
    {
        instances = other.instances;
        count = other.count;
        scale_volume = other.scale_volume;
        is_location = other.is_location;

        other.instances.clear();
    }
    Sound &operator=(Sound &&other)
    {
        if(this == &other) return *this;
        for(auto sound : instances) ma_sound_uninit(sound);
        instances = other.instances;
        count = other.count;
        scale_volume = other.scale_volume;
        is_location = other.is_location;

        other.instances.clear();
        return *this;
    }
    ~Sound()
    {
        for(auto sound : instances) ma_sound_uninit(sound);
    }

//     void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
//     {
//         Audio *audio = world.get_single_comp<Audio>();
//         if(!audio) return;

//         std::string path = std::string(get_var_json<std::string_view>(obj["path"]));

//         count = get_var_json<int64_t>(obj["count"]);
//         instances.resize(count);

//         auto resultDouble = obj["scaleVolume"].get_double();
//         if(!resultDouble.error()) scaleVolume = resultDouble.value();

//         auto resultBool = obj["location"].get_bool();
//         if(!resultBool.error()) isLocation = resultBool.value();

//         for(int i = 0; i < count; i++)
//         {
//             instances[i] = new ma_sound;
//             ma_result resultSound = ma_sound_init_from_file(&audio->engine, path.c_str(), (isLocation ? 0 : MA_SOUND_FLAG_NO_SPATIALIZATION), nullptr, nullptr, instances[i]);
// #ifndef FLAG_RELEASE
//             if(resultSound != MA_SUCCESS)
//             {
//                 std::cerr << "Failed init sould to path " << path << " " << resultSound << std::endl;
//             }
// #endif
//         }

//         if(isLocation)
//         {
//             auto resultInt = obj["maxDistance"].get_int64();
//             if(!resultInt.error())
//                 for(auto sound : instances)
//                     ma_sound_set_max_distance(sound, resultInt.value());
//             else
//                 for(auto sound : instances)
//                     ma_sound_set_max_distance(sound, audio->sound_scale);
//         }
//     }

    void set_volume(float volume)
    {
        for(auto sound : instances)
            ma_sound_set_volume(sound, volume * scale_volume);
    }

    ma_sound *get_free_sound()
    {
        for(auto sound : instances)
            if(!ma_sound_is_playing(sound)) return sound;
        return nullptr;
    }
};

}

#endif
