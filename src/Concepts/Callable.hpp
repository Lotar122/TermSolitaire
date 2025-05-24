#pragma once

#include <concepts>

template<typename Func>
concept Callable = std::is_invocable_v<Func>;