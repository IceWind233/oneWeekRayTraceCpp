cmake_minimum_required(VERSION 3.5)
project(rayTraceCpp)

# NanoGUI build setting
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_SHARED OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

set(RAY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RAY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(3rd_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd)

add_subdirectory(${3rd_DIR}/nanogui)
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")

add_definitions(${NANOGUI_EXTRA_DEFS})
include_directories(${NANOGUI_EXTRA_INCS})
include_directories(${3rd_DIR}/nanogui/include)

include_directories(${RAY_INCLUDE_DIRS})

add_executable(rayTraceCpp main.cpp)

target_link_libraries(rayTraceCpp nanogui ${NANOGUI_EXTRA_LIBS})