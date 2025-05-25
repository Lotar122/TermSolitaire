#pragma once

#include <concepts>
#include <vector>

template<typename T>
concept StdVector = requires {
    typename T::value_type;
    typename T::allocator_type;
    requires std::same_as<T, std::vector<typename T::value_type, typename T::allocator_type>>;
};