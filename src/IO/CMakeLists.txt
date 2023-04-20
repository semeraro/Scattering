## build the IO library
##
cmake_minimum_required(VERSION 3.1)
project(ScatterIO VERSION 0.0.1 LANGUAGES CXX)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
find_package(openvkl REQUIRED)
find_package(rkcommon REQUIRED)
set(VKL_RENDERER_DIR "$ENV{homepath}\\Documents\\VolumeRendering\\openvkl\\examples\\interactive")
set(VKL_TESTING_DIR "$ENV{homepath}\\Documents\\VolumeRendering\\openvkl\\testing")
include_directories( ${VKL_RENDERER_DIR} ${VKL_TESTING_DIR})
#link_directories($ENV{TACC_NETCDF_LIB})

add_library(ScatterIO SHARED ${CMAKE_CURRENT_SOURCE_DIR}/Domain.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ToVKL.cpp)
target_link_libraries(ScatterIO  rkcommon::rkcommon netcdf netcdf_c++ )

add_executable(iotest ${CMAKE_CURRENT_SOURCE_DIR}/iotest.cpp)
target_link_libraries(iotest openvkl::openvkl  ScatterIO)
