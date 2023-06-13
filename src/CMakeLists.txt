## top level cmake lists file for the Scattering project. 
## the project builds the IO, rendering, etc subdirectories 
## from here in the src directory. The build directory is ../build

cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(Scattering LANGUAGES CXX) # the project
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON) # no in sourc builds allowed
# targets created in the subdirectories go to the ../build
# (PROJECT_BINARY_DIR) directory. This keeps all the libs etc in
# one place.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

find_package(openvkl REQUIRED)
find_package(rkcommon REQUIRED)

add_subdirectory(IO) # make the IO library
add_subdirectory(renderer)
add_subdirectory(CloudRender)
#add_executable(vklTutorial ${CMAKE_CURRENT_SOURCE_DIR}/vklTutorial.c)
#target_link_libraries(vklTutorial openvkl::openvkl)

#add_executable(vklTutorialLinkCPUDevice ${CMAKE_CURRENT_SOURCE_DIR}/../vklTutorial.c)
#target_link_libraries(vklTutorialLinkCPUDevice openvkl::openvkl_module_cpu_device)
