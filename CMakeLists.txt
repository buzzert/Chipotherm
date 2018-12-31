cmake_minimum_required(VERSION 3.2)
project(chipotherm)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(CHIPOTHERM_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

# Project specific dependencies
find_package(PkgConfig)
set(PROJECT_DEPS sdl2 SDL2_image freetype2 pango pangoft2 pangocairo cairo)
pkg_check_modules (DEPS REQUIRED ${PROJECT_DEPS})

set(CFLAGS ${DEPS_CFLAGS} ${DEPS_CFLAGS_OTHER})
set(LIB_PATHS ${DEPS_LIBRARY_DIRS})
add_definitions(${CFLAGS})
link_directories(${LIB_PATHS})

# add_subdirectory(lib)

# Some include paths
include_directories(src)

set(SOURCE_FILES
    src/actor.cpp
    src/actor_grid.cpp
    src/cairo_actor.cpp
    src/geometry.cpp
    src/label_actor.cpp
    src/main.cpp
    src/main_scene.cpp
    src/qube_actor.cpp
    src/texture_actor.cpp
)

add_executable(chipotherm ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DEPS_LIBRARIES})
install(TARGETS chipotherm DESTINATION ${CHIPOTHERM_INSTALL_BIN_DIR})

