// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_IOTA_HPP
#define ALLOY_TUPLE_IOTA_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/curry.hpp>
#include <alloy/number/number.hpp>
#include <alloy/tuple/tuple.hpp>

#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <metal/lambda/apply.hpp>
#include <metal/lambda/lambda.hpp>
#include <metal/list/iota.hpp>
#include <metal/number/number.hpp>

namespace alloy::detail {
    template<typename Start, typename Size, typename Stride = number<1>,
        typename Nums = metal::apply<
            metal::lambda<tuple>,
            metal::iota<
                metal::number<value<Start>>,
                metal::number<value<Size>>,
                metal::number<value<Stride>>
            >
        >
    >
    constexpr Nums iota(Start, Size, Stride&& = {}) noexcept {
        return {};
    }

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            iota(static_cast<Args&&>(args)...)
        )
    } _iota{};
}

namespace alloy {
    inline constexpr auto iota = curry<2, 3>(detail::_iota);
}

#endif
