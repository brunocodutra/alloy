// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_LOOKUP_HPP
#define ALLOY_DETAIL_LOOKUP_HPP

#include <alloy/config.hpp>

#include <alloy/detail/traits.hpp>

#define ALLOY_LOOKUP(PROP, OBJ, ...) \
    static_cast<transfer<OBJ, typename PROP::type>>( \
        static_cast<transfer<OBJ, PROP>>( \
            static_cast<OBJ>(__VA_ARGS__) \
        ) \
    ) \
/**/

#endif
