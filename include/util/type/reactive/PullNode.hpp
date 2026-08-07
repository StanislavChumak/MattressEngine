#ifndef PULL_NODE_HPP
#define PULL_NODE_HPP

namespace mtrs::react
{

struct PullNode
{
    void (*_mark_dirty)(void*) = nullptr;
    void* _self = nullptr;

    template <typename Derived>
    static void mark_dirty_trampoline(void* self)
    {
        static_cast<Derived*>(self)->mark_dirty();
    }

    PullNode() = default;

    template <typename Derived>
    PullNode(Derived* self)
    : _mark_dirty(&mark_dirty_trampoline<Derived>)
    , _self(self)
    {}

    void mark_dirty()
    {
        _mark_dirty(_self);
    }
    
};

}

#endif
