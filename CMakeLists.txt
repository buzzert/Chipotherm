cmake_minimum_required(VERSION 3.0.2)

project (chipotherm)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

# libtempered options
set(BUILD_STATIC_LIB ON)

pkg_check_modules(HIDAPI REQUIRED hidapi-hidraw)
include_directories(${HIDAPI_INCLUDE_DIRS})

include_directories(subprojects/TEMPered/libtempered)

add_subdirectory(subprojects/bubbles)
add_subdirectory(subprojects/TEMPered/libtempered)
add_subdirectory(src)

