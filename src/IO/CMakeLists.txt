## build the IO library and test executable
##
cmake_minimum_required(VERSION 3.1)
project(ScatterIO VERSION 0.0.1 LANGUAGES CXX)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
# detect visual studio
# visual studio needs the export all symbols flag
# on to build shared libs. 
if(MSVC)
    message(" doing shared library foo")
    set(BUILD_SHARED_LIBS TRUE)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(MSVC)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(COMPILER_DEFS "")
set(IOLIBS "") 
set(IOINCLUDES "")
find_package(openvkl REQUIRED)
find_package(rkcommon REQUIRED)
#
# options to build IO bits for different file types
#
option(BUILD_NETCDF "build IO routines that support netcdf" OFF)
if(BUILD_NETCDF)
    find_package(netCDF REQUIRED)
    set(COMPILER_DEFS ${COMPILER_DEFS} "WITH_NETCDF=1")
    set(IOLIBS ${IOLIBS} " netcdf netcdf-c++ ")
    set(IOINCLUDES ${IOINCLUDES} ${netCDF_INCLUDE_DIR})
endif(BUILD_NETCDF)
set(VKL_RENDERER_DIR "$ENV{homepath}\\Documents\\VolumeRendering\\openvkl\\examples\\interactive")
set(VKL_TESTING_DIR "$ENV{homepath}\\Documents\\VolumeRendering\\openvkl\\testing")
#include_directories(${openvkl_INCLUDE_DIR} ${VKL_RENDERER_DIR} ${VKL_TESTING_DIR} ${netCDF_INCLUDE_DIR})
message(" found openvkl ${openvkl_DIR}")
link_directories(${netCDF_LIB_DIR})

add_library(ScatterIO SHARED ${CMAKE_CURRENT_SOURCE_DIR}/Domain.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ToVKL.cpp)
TARGET_COMPILE_DEFINITIONS(ScatterIO PUBLIC ${COMPILER_DEFS})
target_include_directories(ScatterIO PUBLIC ${IOINCLUDES})
target_link_libraries(ScatterIO  openvkl::openvkl rkcommon::rkcommon ${IOLIBS})

add_executable(iotest ${CMAKE_CURRENT_SOURCE_DIR}/iotest.cpp)
TARGET_COMPILE_DEFINITIONS(iotest PUBLIC ${COMPILER_DEFS})
target_link_libraries(iotest openvkl::openvkl  ScatterIO)

add_executable(readtest ${CMAKE_CURRENT_SOURCE_DIR}/readfloat.cpp)