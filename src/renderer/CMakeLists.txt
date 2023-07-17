## build the render library and test executable ... maybe
##
cmake_minimum_required(VERSION 3.1)
#project(ScatterIO VERSION 0.0.1 LANGUAGES CXX)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
# detect visual studio
# visual studio needs the export all symbols flag
# on to build shared libs. 
if(MSVC)
    #message(" doing shared library foo")
    set(BUILD_SHARED_LIBS TRUE)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(MSVC)
#
set(COMPILER_DEFS "")
set(IOLIBS "") 
set(IOINCLUDES "")
set(VKL_TESTING_DIR "C:\\$ENV{homepath}\\Documents\\VolumeRendering\\openvkl\\testing")

add_library(Renderers STATIC ${CMAKE_CURRENT_SOURCE_DIR}/RayMarchVolRenderer.cpp 
                            ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp 
                            ${CMAKE_CURRENT_SOURCE_DIR}/TransferFunction.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/PathTraceRenderer.cpp)
TARGET_COMPILE_DEFINITIONS(Renderers PUBLIC ${COMPILER_DEFS})
target_include_directories(Renderers PUBLIC ${IOINCLUDES} ${VKL_TESTING_DIR} ..)
target_link_libraries(Renderers  openvkl::openvkl rkcommon::rkcommon ScatterIO ${TESTING})
add_dependencies(Renderers ScatterIO)

#add_executable(iotest ${CMAKE_CURRENT_SOURCE_DIR}/iotest.cpp)
#TARGET_COMPILE_DEFINITIONS(iotest PUBLIC ${COMPILER_DEFS})
#target_link_libraries(iotest openvkl::openvkl  ScatterIO)

#add_executable(readtest ${CMAKE_CURRENT_SOURCE_DIR}/readfloat.cpp)