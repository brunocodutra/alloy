# Copyright Bruno Dutra 2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

test_units(example Alloy "${CMAKE_CURRENT_SOURCE_DIR}/src/")
add_custom_target(examples DEPENDS example)
