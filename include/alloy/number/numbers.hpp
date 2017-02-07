// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_NUMBER_NUMBERS_HPP
#define ALLOY_NUMBER_NUMBERS_HPP

#include <alloy/config.hpp>
#include <alloy/number/number.hpp>
#include <alloy/tuple/tuple.hpp>

namespace alloy {
    template<auto... Is>
    using numbers = tuple<number<Is>...>;
}

#endif
