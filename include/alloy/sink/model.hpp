#ifndef ALLOY_SINK_MODEL_HPP
#define ALLOY_SINK_MODEL_HPP

#include "../config.hpp"
#include "../detail.hpp"

namespace alloy {
    template<typename S>
    struct sink : detail::invocable<S> {
        using base = detail::invocable<S>;

        using base::base;
        using base::operator();
    };

    template<typename S>
    sink(S &&)->sink<S>;
}

#endif
