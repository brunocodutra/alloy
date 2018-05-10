cmake_minimum_required(VERSION 3.9)

include(cmake/Test.cmake)
include(cmake/Deploy.cmake)

project(Alloy CXX)

set(ALLOY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")

file(STRINGS "${ALLOY_INCLUDE_DIR}/alloy/config/version.hpp"
    ALLOY_CONFIG_VERSION_HPP REGEX "ALLOY_[A-Z]+ [0-9]+" LIMIT_COUNT 3
)

list(GET ALLOY_CONFIG_VERSION_HPP 0 ALLOY_MAJOR)
list(GET ALLOY_CONFIG_VERSION_HPP 1 ALLOY_MINOR)
list(GET ALLOY_CONFIG_VERSION_HPP 2 ALLOY_PATCH)

string(REGEX REPLACE ".*MAJOR ([0-9]+).*" "\\1" ALLOY_MAJOR "${ALLOY_MAJOR}")
string(REGEX REPLACE ".*MINOR ([0-9]+).*" "\\1" ALLOY_MINOR "${ALLOY_MINOR}")
string(REGEX REPLACE ".*PATCH ([0-9]+).*" "\\1" ALLOY_PATCH "${ALLOY_PATCH}")

set(ALLOY_VERSION "${ALLOY_MAJOR}.${ALLOY_MINOR}.${ALLOY_PATCH}")

message(STATUS "Configuring Alloy ${ALLOY_VERSION}")

add_header_library(Alloy "${ALLOY_INCLUDE_DIR}/alloy.hpp" "LICENSE")
set_target_properties(Alloy PROPERTIES
    INTERFACE_LIB_VERSION ${ALLOY_VERSION}
    INTERFACE_COMPILE_FEATURES cxx_std_17
)

deploy_header_library(Alloy)

add_subdirectory(test)
add_subdirectory(example)
