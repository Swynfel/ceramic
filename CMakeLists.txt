cmake_minimum_required(VERSION 3.10)
file(READ "VERSION" version)
project(ceramic VERSION ${version})
message("Building Ceramic version ${version}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB sources CONFIGURE_DEPENDS src/**/*.cpp)
file(GLOB pybind_sources CONFIGURE_DEPENDS src/**/py_*.cpp src/targets/**/*)
list(REMOVE_ITEM sources ${pybind_sources})

add_library(ceramic-core src/targets/lib_core.hpp ${sources})
target_include_directories(ceramic-core PUBLIC src)

add_executable(ceramic-terminal-player src/targets/terminal-player.cpp)
target_include_directories(ceramic-terminal-player PUBLIC src)
target_link_libraries(ceramic-terminal-player PUBLIC ceramic-core)

add_executable(ceramic-arena src/targets/arena.cpp)
target_include_directories(ceramic-arena PUBLIC src)
target_link_libraries(ceramic-arena PUBLIC ceramic-core)
target_link_libraries(ceramic-arena PRIVATE Threads::Threads)

if(PYTHON)
    set(PYBIND11_PYTHON_VERSION ${PYTHON})
elseif($ENV{PYTHON})
    set(PYBIND11_PYTHON_VERSION $ENV{PYTHON})
endif()
if(PYBIND11_PYTHON_VERSION)
    set(PYBIND11_CPP_STANDARD -std=c++14)
    find_package(PythonLibs REQUIRED)
    find_package(pybind11 REQUIRED)
    pybind11_add_module(ceramic src/py_main.cpp ${pybind_sources})
    target_link_libraries(ceramic PUBLIC ceramic-core)
endif()