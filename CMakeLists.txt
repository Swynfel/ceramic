cmake_minimum_required(VERSION 3.10)
project(ceramic VERSION 0.1.6)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB sources CONFIGURE_DEPENDS src/**/*.cpp)
file(GLOB pybind_sources CONFIGURE_DEPENDS src/**/py_*.cpp src/targets/**/*)
list(REMOVE_ITEM sources ${pybind_sources})

add_library(ceramic-core src/targets/lib_core.hpp ${sources})
target_include_directories(ceramic-core PUBLIC src)

add_executable(ceramic-test src/targets/test.cpp)
target_include_directories(ceramic-test PUBLIC src)
target_link_libraries(ceramic-test PUBLIC ceramic-core)

add_executable(ceramic-terminal-player src/targets/terminal-player.cpp)
target_include_directories(ceramic-terminal-player PUBLIC src)
target_link_libraries(ceramic-terminal-player PUBLIC ceramic-core)
