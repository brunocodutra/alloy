// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_TRAITS_HPP
#define ALLOY_DETAIL_TRAITS_HPP

#include "../config.hpp"

#include <type_traits>

namespace alloy::detail {
    inline constexpr enum class valid_t {} valid = {};

    template<auto cond>
    using requires = std::enable_if_t<cond, valid_t>;

    template<typename T>
    using strip = std::remove_cv_t<std::remove_reference_t<T>>;
}

#endif
