#ifndef VEC4_HPP
#define VEC4_HPP

#include "vec.hpp"

namespace mtrs::math
{

template<typename T>
struct vec4
{
    T x,y,z,w;

    template<int C>
    vec3(vec<T, C> &&other)
    {
        operator=(other);
    }

    template<int C>
    vec3<T> &operator=(vec<T, C> &&other)
    {
        static_assert(C >= 4, "conversion is impossible");
        x = other[0];
        y = other[1];
        z = other[2];
        w = other[3];
    }

    VOID_VEC(vec4, x, xy, 0, 1);
    VOID_VEC(vec4, x, xz, 0, 2);
    VOID_VEC(vec4, x, xw, 0, 3);
    VOID_VEC(vec4, x, yx, 1, 0);
    VOID_VEC(vec4, x, yz, 1, 2);
    VOID_VEC(vec4, x, yw, 1, 3);
    VOID_VEC(vec4, x, zx, 2, 0);
    VOID_VEC(vec4, x, zy, 2, 1);
    VOID_VEC(vec4, x, zw, 2, 3);
    VOID_VEC(vec4, x, wx, 3, 0);
    VOID_VEC(vec4, x, wy, 3, 1);
    VOID_VEC(vec4, x, wz, 3, 2);

    VOID_VEC(vec4, x, xyz, 0, 1, 2);
    VOID_VEC(vec4, x, xyw, 0, 1, 3);
    VOID_VEC(vec4, x, xzy, 0, 2, 1);
    VOID_VEC(vec4, x, xzw, 0, 2, 3);
    VOID_VEC(vec4, x, xwy, 0, 3, 1);
    VOID_VEC(vec4, x, xwz, 0, 3, 2);
    VOID_VEC(vec4, x, yxz, 1, 0, 2);
    VOID_VEC(vec4, x, yxw, 1, 0, 3);
    VOID_VEC(vec4, x, yzx, 1, 2, 0);
    VOID_VEC(vec4, x, yzw, 1, 2, 3);
    VOID_VEC(vec4, x, ywx, 1, 3, 0);
    VOID_VEC(vec4, x, ywx, 1, 3, 2);
    VOID_VEC(vec4, x, zxy, 2, 0, 1);
    VOID_VEC(vec4, x, zxw, 2, 0, 3);
    VOID_VEC(vec4, x, zyx, 2, 1, 0);
    VOID_VEC(vec4, x, zyw, 2, 1, 3);
    VOID_VEC(vec4, x, zwx, 2, 3, 0);
    VOID_VEC(vec4, x, zwy, 2, 3, 1);
    VOID_VEC(vec4, x, wxy, 3, 0, 1);
    VOID_VEC(vec4, x, wxz, 3, 0, 2);
    VOID_VEC(vec4, x, wyx, 3, 1, 0);
    VOID_VEC(vec4, x, wyz, 3, 1, 2);
    VOID_VEC(vec4, x, wzx, 3, 2, 0);
    VOID_VEC(vec4, x, wzy, 3, 2, 1);

    VOID_VEC(vec4, x, xyzw, 0, 1, 2, 3);
    VOID_VEC(vec4, x, xywz, 0, 1, 3, 2);
    VOID_VEC(vec4, x, xzyw, 0, 2, 1, 3);
    VOID_VEC(vec4, x, xzwy, 0, 2, 3, 1);
    VOID_VEC(vec4, x, xwyz, 0, 3, 1, 2);
    VOID_VEC(vec4, x, xwzy, 0, 3, 2, 1);
    VOID_VEC(vec4, x, yxzw, 1, 0, 2, 3);
    VOID_VEC(vec4, x, yxwz, 1, 0, 3, 2);
    VOID_VEC(vec4, x, yzxw, 1, 2, 0, 3);
    VOID_VEC(vec4, x, yzwx, 1, 2, 3, 0);
    VOID_VEC(vec4, x, ywxz, 1, 3, 0, 2);
    VOID_VEC(vec4, x, ywxz, 1, 3, 2, 0);
    VOID_VEC(vec4, x, zxyw, 2, 0, 1, 3);
    VOID_VEC(vec4, x, zxwy, 2, 0, 3, 1);
    VOID_VEC(vec4, x, zyxw, 2, 1, 0, 3);
    VOID_VEC(vec4, x, zywx, 2, 1, 3, 0);
    VOID_VEC(vec4, x, zwxy, 2, 3, 0, 1);
    VOID_VEC(vec4, x, zwyx, 2, 3, 1, 0);
    VOID_VEC(vec4, x, wxyz, 3, 0, 1, 2);
    VOID_VEC(vec4, x, wxzy, 3, 0, 2, 1);
    VOID_VEC(vec4, x, wyxz, 3, 1, 0, 2);
    VOID_VEC(vec4, x, wyzx, 3, 1, 2, 0);
    VOID_VEC(vec4, x, wzxy, 3, 2, 0, 1);
    VOID_VEC(vec4, x, wzyx, 3, 2, 1, 0);
};

}

#endif