cmake_minimum_required(VERSION 3.2)
project(chipotherm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(CHIPOTHERM_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

# Project specific dependencies
find_package(PkgConfig)
set(PROJECT_DEPS sdl2)
pkg_check_modules (DEPS REQUIRED ${PROJECT_DEPS})

set(CFLAGS ${DEPS_CFLAGS} ${DEPS_CFLAGS_OTHER})
set(LIB_PATHS ${DEPS_LIBRARY_DIRS})
add_definitions(${CFLAGS})
link_directories(${LIB_PATHS})

# Some include paths
include_directories(src)
# include_directories (${CMAKE_BINARY_DIR}/src)

set(SOURCE_FILES 
    src/main.cpp
)

add_executable(chipotherm ${SOURCE_FILES})
target_link_libraries (${PROJECT_NAME} ${DEPS_LIBRARIES})
install(TARGETS chipotherm DESTINATION ${CHIPOTHERM_INSTALL_BIN_DIR})

