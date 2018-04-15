#ifndef ALLOY_FILTER_MODEL_HPP
#define ALLOY_FILTER_MODEL_HPP

#include "../config.hpp"
#include "../detail.hpp"

namespace alloy {
    template<typename S>
    struct filter : detail::invocable<S> {
        using base = detail::invocable<S>;

        using base::base;
        using base::operator();
    };

    template<typename S>
    filter(S &&)->filter<S>;
}

#endif
