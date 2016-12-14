# Copyright Bruno Dutra 2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
endif()

project(Alloy CXX)

option(ALLOY_STRICT  "treat compiler warnings as errors" OFF)
option(ALLOY_VERBOSE "increase output verbosity"         OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Configure)
include(Test)
include(Deploy)

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

add_header_library(Alloy "${ALLOY_INCLUDE_DIR}/alloy.hpp")
set_target_properties(Alloy PROPERTIES
    INTERFACE_LIB_VERSION ${ALLOY_VERSION}
    INTERFACE_COMPILE_FEATURES cxx_std_17
)

deploy_header_library(Alloy)

set(CMAKE_CXX_EXTENSIONS OFF)

target_compile_flags(Alloy INTERFACE -W)
target_compile_flags(Alloy INTERFACE -Wall)
target_compile_flags(Alloy INTERFACE -Wextra)
target_compile_flags(Alloy INTERFACE -pedantic)

if(ALLOY_STRICT)
    target_compile_flags(Alloy INTERFACE -Werror)
    target_compile_flags(Alloy INTERFACE -pedantic-errors)
endif()

if(ALLOY_VERBOSE)
    target_compile_flags(Alloy INTERFACE -v)
    target_compile_flags(Alloy INTERFACE -fno-elide-type)
    target_compile_flags(Alloy INTERFACE -ftemplate-backtrace-limit=0)
    target_compile_flags(Alloy INTERFACE -fdiagnostics-show-template-tree)
endif()

target_compile_flags(Alloy INTERFACE -stdlib=libc++ LIBS c++ c++abi)

target_compile_flags(Alloy INTERFACE -fno-omit-frame-pointer
    CONFIGS Debug RelWithDebInfo
)

target_compile_flags(Alloy INTERFACE -fsanitize=address,leak,undefined
    LIBS -fsanitize=address,leak,undefined
    CONFIGS Debug RelWithDebInfo
)

add_subdirectory(test)
