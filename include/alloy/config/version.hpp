// Copyright Bruno Dutra 2016
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_CONFIG_VERSION_HPP
#define ALLOY_CONFIG_VERSION_HPP

#define ALLOY_MAJOR 0
#define ALLOY_MINOR 1
#define ALLOY_PATCH 0

#define ALLOY_SEMVER(MAJOR, MINOR, PATCH) \
    (((MAJOR) * 1000000) + ((MINOR) * 10000) + (PATCH))

#define ALLOY_VERSION ALLOY_SEMVER(ALLOY_MAJOR, ALLOY_MINOR, ALLOY_PATCH)

#endif
