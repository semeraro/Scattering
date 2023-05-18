## top level cmake lists file for the Scattering project

cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(Scattering LANGUAGES CXX)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

find_package(openvkl REQUIRED)

add_subdirectory(IO)
#add_executable(vklTutorial ${CMAKE_CURRENT_SOURCE_DIR}/vklTutorial.c)
#target_link_libraries(vklTutorial openvkl::openvkl)

#add_executable(vklTutorialLinkCPUDevice ${CMAKE_CURRENT_SOURCE_DIR}/../vklTutorial.c)
#target_link_libraries(vklTutorialLinkCPUDevice openvkl::openvkl_module_cpu_device)
