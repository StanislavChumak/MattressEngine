#ifndef REACTIVE_SET_HPP
#define REACTIVE_SET_HPP

#include "ReactiveValue.hpp"

namespace mtrs::react
{

template <typename T, size_t MaxObservers = 8, typename ...Deps>
class ReactiveSet : public ReactiveValue<T, MaxObservers, Deps...>
{
public:
    using ElementType = std::decay_t<decltype(std::declval<T>()[0])>;

    ReactiveSet(const ReactiveSet&) = delete;
    ReactiveSet &operator=(const ReactiveSet&) = delete;
    ReactiveSet(ReactiveSet &&) = delete;
    ReactiveSet &operator=(ReactiveSet &&) = delete;
 
    ReactiveSet()
    : ReactiveValue<T, MaxObservers, Deps...>()
    {
    }
 
    explicit ReactiveSet(T initial)
    : ReactiveValue<T, MaxObservers, Deps...>(std::move(initial))
    {
    }
 
    ReactiveSet(T(*calculator)(const Deps* ...), PushNode<Deps>... deps)
    : ReactiveValue<T, MaxObservers, Deps...>(calculator, std::move(deps) ...)
    {
    }

    void set_index(size_t i, ElementType value)
    {
        this->_value[i] = std::move(value);
        this->_dirty = true;
        this->notify_observers();
    }

    const ElementType& get_index(size_t i) const
    {
        return this->_value[i];
    }

    REACTIVE_OPERATORS(ReactiveSet)
};

}

#endif
