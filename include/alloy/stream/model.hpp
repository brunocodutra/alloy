#ifndef ALLOY_STREAM_MODEL_HPP
#define ALLOY_STREAM_MODEL_HPP

#include "../config.hpp"
#include "../detail.hpp"

namespace alloy {
    template<typename S>
    struct stream : detail::invocable<S> {
        using base = detail::invocable<S>;

        using base::base;
        using base::operator();
    };

    template<typename S>
    stream(S &&)->stream<S>;
}

#endif
