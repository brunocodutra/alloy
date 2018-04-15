#ifndef ALLOY_EXTERNAL_HPP
#define ALLOY_EXTERNAL_HPP

#if !defined(METAL_VERSION)
#include "external/metal/metal.hpp"
#endif

#if METAL_VERSION < METAL_SEMVER(0, 7, 0)
#error Unsuported Metal version
#endif

#endif
