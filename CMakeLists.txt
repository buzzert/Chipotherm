cmake_minimum_required(VERSION 3.0.2)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake
)

include(gresource)

project (chipotherm)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)

# libtempered options
set(BUILD_STATIC_LIB ON)

pkg_check_modules(HIDAPI REQUIRED hidapi-hidraw)
include_directories(${HIDAPI_INCLUDE_DIRS})

# Resources
GLIB_COMPILE_RESOURCES(GLIB_RESOURCES
    SOURCE resources/chipotherm.gresource.xml
)

add_library(resources ${GLIB_RESOURCES})
target_include_directories(resources PUBLIC ${GIO_INCLUDE_DIRS})
target_link_libraries(resources PUBLIC ${GIO_LIBRARIES})

include_directories(subprojects/TEMPered/libtempered)

add_subdirectory(subprojects/bubbles)
add_subdirectory(subprojects/TEMPered/libtempered)
add_subdirectory(src)

