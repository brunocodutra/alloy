// Copyright Bruno Dutra 2017-2018
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>

#if EXPECTED_ALLOY_MAJOR != ALLOY_MAJOR
#error major version mismatch!
#endif

#if EXPECTED_ALLOY_MINOR != ALLOY_MINOR
#error minor version mismatch!
#endif

#if EXPECTED_ALLOY_PATCH != ALLOY_PATCH
#error patch version mismatch!
#endif

#define EXPECTED_ALLOY_VERSION ALLOY_SEMVER( \
    EXPECTED_ALLOY_MAJOR, \
    EXPECTED_ALLOY_MINOR, \
    EXPECTED_ALLOY_PATCH \
)

#if EXPECTED_ALLOY_VERSION != ALLOY_VERSION
#error semantic version mismatch!
#endif

int main() {
    return 0;
}
