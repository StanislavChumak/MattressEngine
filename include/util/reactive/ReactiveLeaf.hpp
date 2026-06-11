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

    ReactiveLeaf(ReactiveLeaf &&other)
    {
        _value = std::move(other._value);
        for (size_t i = 0; i < _observer_count; i++)
        {
            _observers[i] = std::move(other._observers[i]);
        }
        _observer_count = other._observer_count;
        other._observer_count = 0;
    }

    ReactiveLeaf &operator=(ReactiveLeaf &&other)
    {
        if(this != &other)
        {
            _value = std::move(other._value);
            for (size_t i = 0; i < _observer_count; i++)
            {
                _observers[i] = std::move(other._observers[i]);
            }
            _observer_count = other._observer_count;
            other._observer_count = 0;
        }
        return *this;
    }

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
        _observers[_observer_count++] = std::move(PullNode<U>(node));
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
