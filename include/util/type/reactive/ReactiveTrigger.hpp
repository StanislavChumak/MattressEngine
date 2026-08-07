#ifndef REACTIVE_TRIGGER_HPP
#define REACTIVE_TRIGGER_HPP

#include "ReactiveBase.hpp"

namespace mtrs::react
{

template <typename ...Deps>
class ReactiveTrigger : public ReactiveBase<ReactiveTrigger<Deps...>, Deps...>
{
    using Base = ReactiveBase<ReactiveTrigger<Deps...>, Deps...>;
    friend Base;

    void (*_callback)(const Deps* ...);

    template <size_t... Is>
    void on_dirty(std::index_sequence<Is...>)
    {
        _callback(std::get<Is>(this->_deps).valid() ? &std::get<Is>(this->_deps).get() : nullptr ...);
    }

public:
    ReactiveTrigger(void(*callback)(const Deps* ...), PushNode<Deps>... deps)
    : Base(deps...), _callback(callback)
    {}

    void set_callback(void(*callback)(const Deps* ...))
    {
        _callback = callback;
        this->_dirty = true;
    }

    void set_callback(void(*callback)(const Deps* ...), PushNode<Deps>... deps)
    {
        _callback = callback;
        this->_deps = std::tuple<PushNode<Deps>...>(deps...);
        this->_dirty = true;
    }
};

}

#endif
