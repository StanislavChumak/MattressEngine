#ifndef VEC2_HPP
#define VEC2_HPP

#include "vec.hpp"

namespace mtrs::math
{

template<typename T>
struct vec2
{
    T x,y;

    vec2()
    {
        static_assert(sizeof(vec2<T>) == sizeof(T)*2,
            "the vector has an unplanned size");
    }

    template<int C>
    vec2(vec<T, C> &&other)
    {
        operator=(other);
    }

    template<int C>
    vec2<T> &operator=(vec<T, C> &&other)
    {
        static_assert(C >= 2, "conversion is impossible");
        x = other[0];
        y = other[1];
    }

    VOID_VEC(vec2, x, xy, 0, 1);
    VOID_VEC(vec2, x, yx, 1, 0);
};

}

#endif
