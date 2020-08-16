#ifndef ALLOY_SOURCE_CAPTURE_HPP
#define ALLOY_SOURCE_CAPTURE_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../source/model.hpp"

namespace alloy::detail {
template <typename... Vs>
constexpr decltype(auto) capture(Vs&&... vs)
{
    return source { detail::args_t<Vs...> { static_cast<Vs&&>(vs)... } };
}
}

namespace alloy {
inline constexpr auto capture = [](auto&&... xs) {
    return detail::capture(static_cast<decltype(xs)>(xs)...);
};
}

#endif
