#ifndef VEC_HPP
#define VEC_HPP

namespace mtrs::math
{

template<typename T, int Count>
struct vec
{
    T data[Count];

    vec() = default;

    vec(T arg)
    {
        for (int i = 0; i < Count; i++)
        {
            data[i] = arg;
        }
    }

    template<int I>
    vec<T, Count> set(T var) noexcept
    {
        data[I] = var;
        return *this;
    }
};

#define VOID_VEC(vec, field, name, ...) [[no_unique_address]] void_vec<decltype(field), __VA_ARGS__> name;\
    static_assert(offsetof(vec, name) == offsetof(vec, field), \
              #name " swizzle member does not overlap "#field" as expected!")

template<typename T, int ...Offsets>
class void_vec
{
    template<int I>
    void set_rec(vec<T, sizeof...(Offsets)> &) noexcept
    {
        
    }

    template<int I, int Offset, int ...Rest>
    void set_rec(vec<T, sizeof...(Offsets)> &v) noexcept
    {
        *(reinterpret_cast<T*>(this) + Offset) = v.data[I];
        set_rec<I+1, Rest...>(v);
    }

    template<int I>
    vec<T, sizeof...(Offsets)> get_rec() const noexcept
    {
        return vec<T, sizeof...(Offsets)>{};
    }

    template<int I, int Offset, int ...Rest>
    vec<T, sizeof...(Offsets)> get_rec() const noexcept
    {
        return get_rec<I+1, Rest...>().template set<I>(*(reinterpret_cast<const T*>(this) + Offset));
    }

public:
    void_vec<T, Offsets...>& operator=(vec<T, sizeof...(Offsets)> rhs) noexcept
    {
        set_rec<0, Offsets...>(rhs);
        return *this;
    }

    operator vec<T, sizeof...(Offsets)>() const noexcept
    {   
        return get_rec<0, Offsets...>();
    }
};

}

#endif
