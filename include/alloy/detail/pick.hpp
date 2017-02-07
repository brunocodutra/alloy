// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_PICK_HPP
#define ALLOY_DETAIL_PICK_HPP

#include <alloy/config.hpp>
#include <alloy/tuple/tuple.hpp>

#include <alloy/detail/lookup.hpp>
#include <alloy/detail/return.hpp>

#include <metal/list/list.hpp>

namespace alloy::detail {
    template<typename Tpl, typename... Props>
    constexpr auto pick(Tpl&& tpl, metal::list<Props...>) ALLOY_RETURN(
        alloy::tuple<transfer_cv<Tpl, typename Props::type>...>(
            ALLOY_LOOKUP(Props, Tpl&&, tpl)...
        )
    )
}

#endif
