## Copyright 2019-2021 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

# NOTE: This CMakeLists.txt is intended to be used to exercise an OpenVKL
#       install and demonstrate how external applications can build against
#       OpenVKL using CMake.
#
#       Once you have done a 'make install' of an existing OpenVKL
#       build, create a separate build directory and invoke CMake on this
#       directory. If you have 'openvkl_DIR' setup correctly to point to where
#       you just installed OpenVKL, then this should built the vklTutorial app
#       from that install and NOT use your local build.

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(vklScatter LANGUAGES CXX)
cmake_policy(SET CMP0074 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
# Set where the OpenVKL examples and testing foo live.
# This is where I pull the rendering and related VKL objects
# that are not really VKL objects. They are part of the testing
# and examples programs. I use them rather than write my own.
# putting in hardcoded paths is probably not a good idea. 
set(VKL_RENDERER_DIR "$ENV{WORK2}/OpenVKL/examples/interactive")
set(VKL_TESTING_DIR "$ENV{WORK2}/OpenVKL/testing")
include_directories(${VKL_RENDERER_DIR} ${VKL_TESTING_DIR})

find_package(openvkl REQUIRED)
find_package(rkcommon REQUIRED)
# now find the rendering library 
find_library(RENDERERS NAMES vkl_example_renderers HINTS "$ENV{WORK2}/OpenVKL/buildgcc/openvkl/build")
# and the testing library
find_library(TESTING NAMES openvkl_testing HINTS "$ENV{WORK2}/OpenVKL/buildgcc/openvkl/build")

add_executable(vklScatter ${CMAKE_CURRENT_SOURCE_DIR}/Scatter.cpp)
target_link_libraries(vklScatter openvkl::openvkl rkcommon::rkcommon ${RENDERERS} ${TESTING})

#add_executable(vklTutorialLinkCPUDevice ${CMAKE_CURRENT_SOURCE_DIR}/../vklTutorial.c)
#target_link_libraries(vklTutorialLinkCPUDevice openvkl::openvkl_module_cpu_device)
