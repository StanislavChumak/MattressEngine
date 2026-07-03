#ifndef REACTIVE_VALUE_HPP
#define REACTIVE_VALUE_HPP

#include "ReactiveBase.hpp"

#include "util/concept.hpp"

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

    template <typename U = T, enable_if_addable<U> = 0>
    auto operator+(const T& rhs) const { return _value + rhs; }

    template <typename U = T, enable_if_subtractable<U> = 0>
    auto operator-(const T& rhs) const { return _value - rhs; }

    template <typename U = T, enable_if_multiplicable<U> = 0>
    auto operator*(const T& rhs) const { return _value * rhs; }

    template <typename U = T, enable_if_divisible<U> = 0>
    auto operator/(const T& rhs) const { return _value / rhs; }


    template <typename U = T, enable_if_addable<U> = 0>
    auto operator+(const ReactiveValue& rhs) const { return _value + rhs._value; }

    template <typename U = T, enable_if_subtractable<U> = 0>
    auto operator-(const ReactiveValue& rhs) const { return _value - rhs._value; }

    template <typename U = T, enable_if_multiplicable<U> = 0>
    auto operator*(const ReactiveValue& rhs) const { return _value * rhs._value; }

    template <typename U = T, enable_if_divisible<U> = 0>
    auto operator/(const ReactiveValue& rhs) const { return _value / rhs._value; }


    template <typename U = T, enable_if_eq<U> = 0>
    bool operator==(const T& rhs) const { return _value == rhs; }

    template <typename U = T, enable_if_eq<U> = 0>
    bool operator!=(const T& rhs) const { return _value != rhs; }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator< (const T& rhs) const { return _value <  rhs; }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator> (const T& rhs) const { return rhs    <  _value; }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator<=(const T& rhs) const { return !(_value > rhs); }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator>=(const T& rhs) const { return !(_value < rhs); }


    template <typename U = T, enable_if_eq<U> = 0>
    bool operator==(const ReactiveValue& rhs) const { return _value == rhs._value; }

    template <typename U = T, enable_if_eq<U> = 0>
    bool operator!=(const ReactiveValue& rhs) const { return _value != rhs._value; }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator< (const ReactiveValue& rhs) const { return _value  < rhs._value; }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator> (const ReactiveValue& rhs) const { return rhs._value < _value; }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator<=(const ReactiveValue& rhs) const { return !(_value > rhs._value); }

    template <typename U = T, enable_if_lt<U> = 0>
    bool operator>=(const ReactiveValue& rhs) const { return !(_value < rhs._value); }


    template <typename U = T, size_t N = sizeof...(Deps), enable_if_addable<U> = 0, enable_if_leaf<N> = 0>
    ReactiveValue& operator+=(const T& rhs) { set(_value + rhs); return *this; }

    template <typename U = T, size_t N = sizeof...(Deps), enable_if_subtractable<U> = 0, enable_if_leaf<N> = 0>
    ReactiveValue& operator-=(const T& rhs) { set(_value - rhs); return *this; }

    template <typename U = T, size_t N = sizeof...(Deps), enable_if_multiplicable<U> = 0, enable_if_leaf<N> = 0>
    ReactiveValue& operator*=(const T& rhs) { set(_value * rhs); return *this; }

    template <typename U = T, size_t N = sizeof...(Deps), enable_if_divisible<U> = 0, enable_if_leaf<N> = 0>
    ReactiveValue& operator/=(const T& rhs) { set(_value / rhs); return *this; }

};

}

#endif
