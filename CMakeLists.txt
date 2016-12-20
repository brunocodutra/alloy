# Copyright Bruno Dutra 2016
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")
endif()

project(Alloy CXX)

set(ALLOY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library(Alloy INTERFACE)
set_target_properties(Alloy PROPERTIES
    INTERFACE_COMPILE_FEATURES cxx_std_17
    INTERFACE_INCLUDE_DIRECTORIES ${ALLOY_INCLUDE_DIR}
)
