#ifndef PUSH_NODE_HPP
#define PUSH_NODE_HPP

#include <cstddef>

namespace mtrs::util
{

template <typename T>
struct PushNode
{
    const T* (*_get)(const void*) = nullptr;
    bool (*_update)(void*) = nullptr;
    void* _self = nullptr;

    template <typename Derived>
    static const T* get_trampoline(const void* self)
    {
        return &static_cast<const Derived*>(self)->get();
    }

    template <typename Derived>
    static bool update_trampoline(void* self)
    {
        return static_cast<Derived*>(self)->update();
    }

    PushNode() = default;
    PushNode(std::nullptr_t) {}

    template <typename Derived>
    PushNode(Derived* self)
    : _get(&get_trampoline<Derived>)
    , _update(&update_trampoline<Derived>)
    , _self(self)
    {}

    bool update()
    {
        return _update(_self);
    }

    const T& get() const
    {
        return *_get(_self);
    }

    bool valid() const
    {
        return _self != nullptr;
    }
};

}

#endif
