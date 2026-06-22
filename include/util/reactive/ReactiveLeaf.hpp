#ifndef REACTIVE_LEAF_HPP
#define REACTIVE_LEAF_HPP

#include "PullNode.hpp"

#include <utility>

namespace mtrs::util
{

template<typename T, typename U = T, size_t MaxObservers = 8>
class ReactiveLeaf
{
    T _value;
    PullNode<U> _observers[MaxObservers];
    size_t _observer_count = 0;

public:
    ~ReactiveLeaf() = default;
    ReactiveLeaf(const ReactiveLeaf&) = delete;
    ReactiveLeaf &operator=(const ReactiveLeaf&) = delete;
    ReactiveLeaf(ReactiveLeaf &&) = delete;
    ReactiveLeaf &operator=(ReactiveLeaf &&) = delete;

    ReactiveLeaf()
    : _value{}
    {}

    ReactiveLeaf(T initial)
    : _value(std::move(initial))
    {}

    void set(T value)
    {
        _value = std::move(value);
        for (size_t i = 0; i < _observer_count; ++i)
            _observers[i].mark_dirty();
    }

    bool add_observer(PullNode<U> node)
    {
        if (_observer_count >= MaxObservers) return false;
        _observers[_observer_count++] = PullNode<U>(node);
        return true;
    }

    bool update() noexcept
    {
        return false;
    }

    const T& get() const noexcept
    {
        return _value;
    }
};

}

#endif
