# Copyright Bruno Dutra 2016-2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

add_custom_target(test.header)
header_test_tree(Alloy test.header ${ALLOY_INCLUDE_DIR})
