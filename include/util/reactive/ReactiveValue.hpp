#ifndef REACTIVE_VALUE_HPP
#define REACTIVE_VALUE_HPP

#include "ReactiveBase.hpp"

namespace mtrs::util
{

template <typename T, typename ...Deps>
class ReactiveValue : public ReactiveBase<ReactiveValue<T, Deps...>, Deps...>
{
    using Base = ReactiveBase<ReactiveValue<T, Deps...>, Deps...>;
    friend Base;

    T _value;
    T (*_calculator)(const Deps* ...);

    template <size_t... Is>
    void on_dirty(std::index_sequence<Is...>)
    {
        _value = _calculator(std::get<Is>(this->_deps).valid() ?
            &std::get<Is>(this->_deps).get() : nullptr ...);
    }

public:
    ReactiveValue(const ReactiveValue&) = delete;
    ReactiveValue &operator=(const ReactiveValue&) = delete;
    ReactiveValue(ReactiveValue &&) = delete;
    ReactiveValue &operator=(ReactiveValue &&) = delete;

    ReactiveValue()
    : _value{}, _calculator(nullptr)
    {
        static_assert(sizeof...(Deps) == 0, "Leaf node can't have dependencies");
        this->_dirty = true;
    }

    explicit ReactiveValue(T initial)
    : _value(std::move(initial)), _calculator(nullptr)
    {
        static_assert(sizeof...(Deps) == 0, "Leaf node can't have dependencies");
        this->_dirty = false;
    }

    ReactiveValue(T(*calculator)(const Deps* ...), PushNode<Deps>... deps)
    : Base(deps...), _value{}, _calculator(calculator)
    {}

    void set(T value)
    {
        static_assert(sizeof...(Deps) == 0, "Only leaf nodes can be set directly");
        _value = std::move(value);
        this->_dirty = true;
    }

    void set_calculator(T(*calculator)(const Deps* ...))
    {
        static_assert(sizeof...(Deps) != 0, "Leaf nodes don't have a calculator");
        _calculator = calculator;
        this->_dirty = true;
    }

    bool update()
    {
        if constexpr (sizeof...(Deps) == 0)
        {
            bool result = this->_dirty;
            this->_dirty = false;
            return result;
        }
        else
        {
            return Base::update();
        }
    }

    const T& get() const noexcept
    {
        return _value;
    }
};

}

#endif
