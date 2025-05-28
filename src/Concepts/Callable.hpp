#pragma once

#include <type_traits>

template<typename T>
concept FunctionPointer = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>;

template<typename T>
concept HasCallOperator = requires(T t) { &T::operator(); };

template<typename T>
concept Callable = FunctionPointer<T> || HasCallOperator<T>;