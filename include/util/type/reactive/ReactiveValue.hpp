#ifndef REACTIVE_VALUE_HPP
#define REACTIVE_VALUE_HPP

#include "ReactiveBase.hpp"
#include "PullNode.hpp"

#include "util/type/concept.hpp"
#include "util/func/mtrs_message.hpp"

namespace mtrs::react
{

template <typename T, size_t MaxObservers = 8, typename ...Deps>
class ReactiveValue : public ReactiveBase<ReactiveValue<T, MaxObservers, Deps...>, Deps...>
{
    using Base = ReactiveBase<ReactiveValue<T, MaxObservers, Deps...>, Deps...>;
    friend Base;
 
    template <size_t... Is>
    void on_dirty(std::index_sequence<Is...>)
    {
        _value = _calculator(std::get<Is>(this->_deps).valid() ?
            &std::get<Is>(this->_deps).get() : nullptr ...);
        notify_observers();
    }

protected:
    T _value;
    T (*_calculator)(const Deps* ...);
 
    PullNode _observers[MaxObservers];
    size_t _observer_count = 0;

    void notify_observers()
    {
        for (size_t i = 0; i < _observer_count; ++i)
            _observers[i].mark_dirty();
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
    {
    }
 
    void set(T value)
    {
        static_assert(sizeof...(Deps) == 0, "Only leaf nodes can be set directly");
        _value = std::move(value);
        this->_dirty = true;
        notify_observers();
    }

    void modify(void(*modifier)(T&))
    {
        static_assert(sizeof...(Deps) == 0, "Only leaf nodes can be set directly");
        modifier(_value);
        this->_dirty = true;
        notify_observers();
    }
    
    bool add_observer(PullNode node)
    {
        if (_observer_count >= MaxObservers)
        {
            return false;
        }
        _observers[_observer_count++] = node;
        return true;
    }
 
    bool remove_observer(void* self)
    {
        for (size_t i = 0; i < _observer_count; ++i)
        {
            if (_observers[i]._self == self)
            {
                for (size_t j = i; j < _observer_count - 1; ++j)
                    _observers[j] = _observers[j + 1];
                --_observer_count;
                return true;
            }
        }
        return false;
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
 
    const T &get() const noexcept
    {
        return _value;
    }

    operator T() const noexcept
    {
        return _value;
    }

    const T* operator->() const noexcept
    {
        return &_value;
    }

#define REACTIVE_OPERATORS(ReactiveType)\
    template <typename U = T, util::enable_if_addable<U> = 0>\
    auto operator+(const T& rhs) const { return this->_value + rhs; }\
    template <typename U = T, util::enable_if_subtractable<U> = 0>\
    auto operator-(const T& rhs) const { return this->_value - rhs; }\
    template <typename U = T, util::enable_if_multiplicable<U> = 0>\
    auto operator*(const T& rhs) const { return this->_value * rhs; }\
    template <typename U = T, util::enable_if_divisible<U> = 0>\
    auto operator/(const T& rhs) const { return this->_value / rhs; }\
\
    template <typename U = T, util::enable_if_addable<U> = 0>\
    auto operator+(const ReactiveType& rhs) const { return this->_value + rhs._value; }\
    template <typename U = T, util::enable_if_subtractable<U> = 0>\
    auto operator-(const ReactiveType& rhs) const { return this->_value - rhs._value; }\
    template <typename U = T, util::enable_if_multiplicable<U> = 0>\
    auto operator*(const ReactiveType& rhs) const { return this->_value * rhs._value; }\
    template <typename U = T, util::enable_if_divisible<U> = 0>\
    auto operator/(const ReactiveType& rhs) const { return this->_value / rhs._value; }\
\
    template <typename U = T, util::enable_if_eq<U> = 0>\
    bool operator==(const T& rhs) const { return this->_value == rhs; }\
    template <typename U = T, util::enable_if_eq<U> = 0>\
    bool operator!=(const T& rhs) const { return this->_value != rhs; }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator< (const T& rhs) const { return this->_value <  rhs; }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator> (const T& rhs) const { return rhs < this->_value; }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator<=(const T& rhs) const { return !(this->_value > rhs); }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator>=(const T& rhs) const { return !(this->_value < rhs); }\
\
    template <typename U = T, util::enable_if_eq<U> = 0>\
    bool operator==(const ReactiveType& rhs) const { return this->_value == rhs._value; }\
    template <typename U = T, util::enable_if_eq<U> = 0>\
    bool operator!=(const ReactiveType& rhs) const { return this->_value != rhs._value; }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator< (const ReactiveType& rhs) const { return this->_value  < rhs._value; }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator> (const ReactiveType& rhs) const { return rhs._value < this->_value; }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator<=(const ReactiveType& rhs) const { return !(this->_value > rhs._value); }\
    template <typename U = T, util::enable_if_lt<U> = 0>\
    bool operator>=(const ReactiveType& rhs) const { return !(this->_value < rhs._value); }\
\
    ReactiveType& operator=(const T& rhs) { this->set(rhs); return *this; }\
    template <typename U = T, size_t N = sizeof...(Deps), util::enable_if_addable<U> = 0, util::enable_if_leaf<N> = 0>\
    ReactiveType& operator+=(const T& rhs) { this->set(this->_value + rhs); return *this; }\
    template <typename U = T, size_t N = sizeof...(Deps), util::enable_if_subtractable<U> = 0, util::enable_if_leaf<N> = 0>\
    ReactiveType& operator-=(const T& rhs) { this->set(this->_value - rhs); return *this; }\
    template <typename U = T, size_t N = sizeof...(Deps), util::enable_if_multiplicable<U> = 0, util::enable_if_leaf<N> = 0>\
    ReactiveType& operator*=(const T& rhs) { this->set(this->_value * rhs); return *this; }\
    template <typename U = T, size_t N = sizeof...(Deps), util::enable_if_divisible<U> = 0, util::enable_if_leaf<N> = 0>\
    ReactiveType& operator/=(const T& rhs) { this->set(this->_value / rhs); return *this; }

    REACTIVE_OPERATORS(ReactiveValue)
};

}

#endif
