// Copyright Bruno Dutra 2017-2018
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_EXTERNAL_HPP
#define ALLOY_EXTERNAL_HPP

#if !defined(METAL_VERSION)
#include "external/metal/metal.hpp"
#endif

#if METAL_VERSION < METAL_SEMVER(0, 7, 0)
#error Unsuported Metal version
#endif

#endif
