#ifndef ALLOY_CONSTANT_HPP
#define ALLOY_CONSTANT_HPP

#include "config.hpp"

#include <type_traits>

namespace alloy {
template <auto c>
using constant = std::integral_constant<decltype(c), c>;
}

#endif
