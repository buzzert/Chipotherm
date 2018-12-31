cmake_minimum_required(VERSION 3.2)
project(chipotherm)

find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CHIPOTHERM_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(CFLAGS ${DEPS_CFLAGS} ${DEPS_CFLAGS_OTHER})
set(LIB_PATHS ${DEPS_LIBRARY_DIRS})

add_definitions(${CFLAGS})
link_directories(${LIB_PATHS})

add_subdirectory(lib/bubbles)

# Sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_executable(chipotherm ${SOURCES})
target_link_libraries(${PROJECT_NAME} bubbles)

install(TARGETS chipotherm DESTINATION ${CHIPOTHERM_INSTALL_BIN_DIR})
