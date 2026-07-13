#ifndef VEC3_HPP
#define VEC3_HPP

#include "vec.hpp"

namespace mtrs::math
{

template<typename T>
struct vec3
{
    T x,y,z;

    template<int C>
    vec3(vec<T, C> &&other)
    {
        operator=(other);
    }

    template<int C>
    vec3<T> &operator=(vec<T, C> &&other)
    {
        static_assert(C >= 3, "conversion is impossible");
        x = other[0];
        y = other[1];
        z = other[2];
    }

    VOID_VEC(vec3, x, xy, 0, 1);
    VOID_VEC(vec3, x, xz, 0, 2);
    VOID_VEC(vec3, x, yx, 1, 0);
    VOID_VEC(vec3, x, yz, 1, 2);
    VOID_VEC(vec3, x, zx, 2, 0);
    VOID_VEC(vec3, x, zy, 2, 1);

    VOID_VEC(vec3, x, xyz, 0, 1, 2);
    VOID_VEC(vec3, x, xzy, 0, 2, 1);
    VOID_VEC(vec3, x, yxz, 1, 0, 2);
    VOID_VEC(vec3, x, yzx, 1, 2, 0);
    VOID_VEC(vec3, x, zxy, 2, 0, 1);
    VOID_VEC(vec3, x, zyx, 2, 1, 0);
};

}

#endif
