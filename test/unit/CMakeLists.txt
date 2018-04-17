add_definitions(-DEXPECTED_ALLOY_MAJOR=${ALLOY_MAJOR})
add_definitions(-DEXPECTED_ALLOY_MINOR=${ALLOY_MINOR})
add_definitions(-DEXPECTED_ALLOY_PATCH=${ALLOY_PATCH})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

test_units(test.unit Alloy "${CMAKE_CURRENT_SOURCE_DIR}/src")
