#ifndef KEY_BUTTONS_HPP
#define KEY_BUTTONS_HPP

namespace mtrs::comp
{

struct KeyButtons
{
    bool keys[349];
    bool last_keys[349];

    KeyButtons() = delete;
    ~KeyButtons() = default;
    KeyButtons(const KeyButtons&) = delete;
    KeyButtons &operator=(const KeyButtons&) = delete;
    KeyButtons(KeyButtons&&) = delete;
    KeyButtons &operator=(KeyButtons&&) = delete;
    
    KeyButtons(void*);
};


}

#endif
