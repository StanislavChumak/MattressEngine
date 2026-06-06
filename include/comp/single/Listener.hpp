#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <cstdint>

namespace mtrs::comp
{

typedef uint64_t EntityID;

struct Listener
{
    EntityID target;
};

}

#endif
