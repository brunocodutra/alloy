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
