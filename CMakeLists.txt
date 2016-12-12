# Copyright Bruno Dutra 2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
endif()

project(Alloy CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Deploy)

set(ALLOY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_header_library(Alloy "${ALLOY_INCLUDE_DIR}/alloy.hpp")
set_target_properties(Alloy PROPERTIES
    INTERFACE_COMPILE_FEATURES cxx_std_17
)
