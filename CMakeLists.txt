# Copyright Bruno Dutra 2016
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")
endif()

project(Alloy CXX)

option(ALLOY_STRICT  "treat compiler warnings as errors" OFF)
option(ALLOY_VERBOSE "increase output verbosity"         OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(TestTree)
include(CompileFlags)

set(ALLOY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library(Alloy INTERFACE)
set_target_properties(Alloy PROPERTIES
    INTERFACE_COMPILE_FEATURES cxx_std_17
    INTERFACE_INCLUDE_DIRECTORIES ${ALLOY_INCLUDE_DIR}
)

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

enable_testing()

add_subdirectory(test)
