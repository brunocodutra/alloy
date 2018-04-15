#ifndef ALLOY_CONFIG_VERSION_HPP
#define ALLOY_CONFIG_VERSION_HPP

#define ALLOY_MAJOR 0
#define ALLOY_MINOR 2
#define ALLOY_PATCH 0

#define ALLOY_SEMVER(MAJOR, MINOR, PATCH) \
    (((MAJOR)*1000000) + ((MINOR)*10000) + (PATCH))

#define ALLOY_VERSION ALLOY_SEMVER(ALLOY_MAJOR, ALLOY_MINOR, ALLOY_PATCH)

#endif
