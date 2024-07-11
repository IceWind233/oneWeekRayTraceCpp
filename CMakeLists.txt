cmake_minimum_required(VERSION 3.5)
project(rayTraceCpp)

set(RAY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RAY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${RAY_INCLUDE_DIRS})

add_executable(rayTraceCpp main.cpp)