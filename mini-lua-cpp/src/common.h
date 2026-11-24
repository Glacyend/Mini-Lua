#ifndef __COMMON_H__
#define __COMMON_H__

#include <type_traits>
#include <variant>

template <typename T, typename Variant>
struct variant_contains : std::false_type {};

template <typename T, typename... Ts>
struct variant_contains<T, std::variant<Ts...>> :
    std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

template <typename T, typename Variant>
inline constexpr bool variant_contains_v = variant_contains<T, Variant>::value;

template <typename T, typename Variant>
concept variant_alternative_of = variant_contains_v<T, Variant>;

#endif
