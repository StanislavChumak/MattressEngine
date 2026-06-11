#ifndef REACTIVE_BASE_HPP
#define REACTIVE_BASE_HPP

#include "PushNode.hpp"
#include <tuple>

namespace mtrs::util
{

template <typename Derived, typename ...Deps>
class ReactiveBase
{
protected:
    [[no_unique_address]] std::tuple<PushNode<Deps>...> _deps;
    bool _dirty = true;

    template <size_t... Is>
    bool update_deps(std::index_sequence<Is...>)
    {
        bool any_dirty = false;
        ((std::get<Is>(_deps).valid() && std::get<Is>(_deps).update() ?
            any_dirty = true : false), ...);
        return any_dirty;
    }

public:
    ~ReactiveBase() = default;
    ReactiveBase(const ReactiveBase &) = delete;
    ReactiveBase &operator=(const ReactiveBase&) = delete;

    ReactiveBase(ReactiveBase &&other) noexcept
    {
        _deps = std::move(_deps);
        _dirty = other._dirty;
    }

    ReactiveBase &operator=(ReactiveBase &&other) noexcept
    {
        if(this != &other)
        {
            _deps = std::move(_deps);
            _dirty = other._dirty;
        }
        return *this;
    }

    ReactiveBase(PushNode<Deps>... deps)
    : _deps(deps...)
    {
    }

    template <size_t I>
    bool valid_dep()
    {
        static_assert(I < sizeof...(Deps), "Index out of range");
        return std::get<I>(_deps).valid();
    }

    template <size_t I, typename Src>
    bool set_dep(Src* src)
    {
        static_assert(I < sizeof...(Deps), "Index out of range");
        auto& slot = std::get<I>(_deps);
        if (slot._self == src) return false;
        slot = PushNode<std::tuple_element_t<I, std::tuple<Deps...>>>(src);
        _dirty = true;
        return true;
    }

    template <size_t I>
    bool clear_dep()
    {
        static_assert(I < sizeof...(Deps), "Index out of range");
        auto& slot = std::get<I>(_deps);
        if (!slot.valid()) return false;
        slot = {};
        _dirty = true;
        return true;
    }

    bool update()
    {
        if (_dirty || update_deps(std::index_sequence_for<Deps...>{}))
        {
            _dirty = false;
            static_cast<Derived*>(this)->on_dirty(std::index_sequence_for<Deps...>{});
            return true;
        }
        return false;
    }

    void mark_dirty()
    {
        _dirty = true;
    }
};

}

#endif
