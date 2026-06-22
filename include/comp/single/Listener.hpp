#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "comp/core/Transform.hpp"

namespace mtrs::comp
{

struct Listener
{
    Transform *target;

    Listener() = delete;
    ~Listener() = default;
    Listener(const Listener&) = delete;
    Listener &operator=(const Listener&) = delete;
    Listener(Listener&&) = delete;
    Listener &operator=(Listener&&) = delete;

    Listener(void*);
};

}

#endif
