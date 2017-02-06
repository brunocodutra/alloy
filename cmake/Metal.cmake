# Copyright Bruno Dutra 2016-2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

find_package(Metal QUIET)
if(Metal_FOUND)
    message(STATUS "Local Metal installation found - version ${Metal_VERSION}")
else()
    message(STATUS "No local Metal installation found - fetching branch master")
    include(ExternalProject)
    ExternalProject_Add(Metal.fetch
        URL "https://github.com/brunocodutra/metal/archive/master.zip"
        TIMEOUT 120
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/metal"
        CMAKE_GENERATOR ${CMAKE_GENERATOR}
        CMAKE_ARGS
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
    ExternalProject_Get_Property(Metal.fetch INSTALL_DIR)
    set(Metal_INCLUDE_DIR "${INSTALL_DIR}/include")
endif()
