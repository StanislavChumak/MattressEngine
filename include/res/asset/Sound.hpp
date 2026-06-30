#ifndef SOUND_HPP
#define SOUND_HPP

#include "res/asset/Asset.hpp"

#include <iosfwd>

namespace mtrs::res
{

class Sound : public Asset<Sound>
{
    struct Impl;
    Impl *_impl;
    std::string _path;
    uint32_t _flag;
    size_t _distance;

    bool _is_init = false;
    size_t _robin = 0;

    void *get_free_sound();

public:
    Sound(std::ifstream &file);
    Sound() = delete;
    Sound(const Sound&) = delete;
    Sound &operator=(const Sound&) = delete;
    Sound(Sound &&other) noexcept;
    Sound &operator=(Sound &&other) noexcept;
    ~Sound();

    static std::string get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    bool init(void *audio_engine);

    void set_volume(float volume);

    void start();
    void play();
    void stop();
};

}

#endif
