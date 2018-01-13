// Copyright Bruno Dutra 2017-2018
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_CONSTANT_HPP
#define ALLOY_CONSTANT_HPP

#include "config.hpp"

#include <type_traits>

namespace alloy {
    template<auto c>
    using constant = std::integral_constant<decltype(c), c>;
}

#endif
