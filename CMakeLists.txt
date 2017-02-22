cmake_minimum_required(VERSION 2.6)

project(TaskScheduler)

# set compilers
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_definitions(-std=c++11)

# include .h's on final compilators project
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(/MinGW/include/c++/6.3.0/)

# add all cpps to one variable
file(GLOB CPP_SOURCES ${PROJECT_SOURCE_DIR}/src/*)

# create the library
add_library(TaskScheduler-lib STATIC ${CPP_SOURCES})

# create a test executable 
add_executable(TaskScheduler test/main.cpp ${CPP_SOURCES})