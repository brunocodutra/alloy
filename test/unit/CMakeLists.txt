# Copyright Bruno Dutra 2016
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_definitions(-DEXPECTED_ALLOY_MAJOR=${ALLOY_MAJOR})
add_definitions(-DEXPECTED_ALLOY_MINOR=${ALLOY_MINOR})
add_definitions(-DEXPECTED_ALLOY_PATCH=${ALLOY_PATCH})

add_custom_target(test.unit)
unit_test_tree(Alloy test.unit "${CMAKE_CURRENT_SOURCE_DIR}/src")
