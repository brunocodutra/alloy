# Copyright Bruno Dutra 2016-2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

test_deployment(test.deployment Alloy alloy.hpp)
test_build_and_run(test.deployment)
