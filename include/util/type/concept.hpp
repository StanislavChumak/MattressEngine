#ifndef CONCEPT_HPP
#define CONCEPT_HPP

#include <type_traits>

namespace mtrs::util
{

template <typename T, typename = void>
struct is_addable : std::false_type {};
template <typename T>
struct is_addable<T, std::void_t<decltype(std::declval<T>() + std::declval<T>())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_subtractable : std::false_type {};
template <typename T>
struct is_subtractable<T, std::void_t<decltype(std::declval<T>() - std::declval<T>())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_multiplicable : std::false_type {};
template <typename T>
struct is_multiplicable<T, std::void_t<decltype(std::declval<T>() * std::declval<T>())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_divisible : std::false_type {};
template <typename T>
struct is_divisible<T, std::void_t<decltype(std::declval<T>() / std::declval<T>())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_equality_comparable : std::false_type {};
template <typename T>
struct is_equality_comparable<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_less_comparable : std::false_type {};
template <typename T>
struct is_less_comparable<T, std::void_t<decltype(std::declval<T>() < std::declval<T>())>>
    : std::true_type {};

template <typename T>
using enable_if_addable = std::enable_if_t<is_addable<T>::value, int>;
template <typename T>
using enable_if_subtractable = std::enable_if_t<is_subtractable<T>::value, int>;
template <typename T>
using enable_if_multiplicable = std::enable_if_t<is_multiplicable<T>::value, int>;
template <typename T>
using enable_if_divisible = std::enable_if_t<is_divisible<T>::value, int>;
template <typename T>
using enable_if_eq = std::enable_if_t<is_equality_comparable<T>::value, int>;
template <typename T>
using enable_if_lt = std::enable_if_t<is_less_comparable<T>::value, int>;

template <size_t N>
using enable_if_leaf = std::enable_if_t<N == 0, int>;

}

#endif
