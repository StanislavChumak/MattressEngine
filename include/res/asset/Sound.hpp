#ifndef SOUND_HPP
#define SOUND_HPP

#include "res/Resource.hpp"

#include <string>

namespace mtrs::res
{

class Sound : public Resource<Sound>
{
    struct Impl *_impl;
    std::string _path;
    uint32_t _flag;
    size_t _distance;

    bool _is_init = false;
    size_t _robin = 0;

    void *get_free_sound();

public:
    Sound(RESOURCE_ARGS);
    Sound() = delete;
    Sound(const Sound&) = delete;
    Sound &operator=(const Sound&) = delete;
    Sound(Sound &&other) noexcept;
    Sound &operator=(Sound &&other) noexcept;
    ~Sound();

    static const char *get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    bool init(void *audio_engine);

    void set_volume(float volume);

    void start();
    void play();
    void stop();
};

}

#endif
