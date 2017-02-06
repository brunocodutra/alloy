// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_SIZE_HPP
#define ALLOY_TUPLE_SIZE_HPP

#include <alloy/config.hpp>
#include <alloy/number/number.hpp>

#include <alloy/detail/object.hpp>
#include <alloy/detail/return.hpp>

namespace alloy::detail {
    template<typename... Props>
    constexpr number<sizeof...(Props)> size(object<Props...> const&) noexcept {
        return {};
    }

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            size(static_cast<Args&&>(args)...)
        )
    } _size{};
}

namespace alloy {
    inline constexpr auto& size = detail::_size;
}

#endif
