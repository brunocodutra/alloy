#ifndef ALLOY_SOURCE_MODEL_HPP
#define ALLOY_SOURCE_MODEL_HPP

#include "../config.hpp"
#include "../detail.hpp"

namespace alloy {
    template<typename S>
    struct source : detail::invocable<S> {
        using base = detail::invocable<S>;

        using base::base;
        using base::operator();
    };

    template<typename S>
    source(S &&)->source<S>;
}

#endif
