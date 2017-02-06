# Copyright Bruno Dutra 2016-2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.1)

project(external CXX)

find_package(Alloy ${ALLOY_VERSION} EXACT REQUIRED)
include_directories(${Alloy_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(external main.cpp)
