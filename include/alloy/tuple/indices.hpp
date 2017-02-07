// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_INDICES_HPP
#define ALLOY_TUPLE_INDICES_HPP

#include <alloy/config.hpp>
#include <alloy/number/number.hpp>
#include <alloy/tuple/iota.hpp>
#include <alloy/tuple/size.hpp>

#include <alloy/detail/return.hpp>

namespace alloy::detail {
    template<typename Tpl>
    constexpr auto indices(Tpl&& tpl) ALLOY_RETURN(
        iota(number<0>{}, size(static_cast<Tpl&&>(tpl)))
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            indices(static_cast<Args&&>(args)...)
        )
    } _indices{};
}

namespace alloy {
    inline constexpr auto& indices = detail::_indices;
}

#endif
