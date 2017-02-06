// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_RETURN_HPP
#define ALLOY_DETAIL_RETURN_HPP

#include <alloy/config.hpp>

#define ALLOY_RETURN(...) noexcept(noexcept(__VA_ARGS__)) \
    -> decltype(__VA_ARGS__) { \
        return __VA_ARGS__; \
    } \
/**/

#endif
