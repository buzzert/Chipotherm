pkg_check_modules(SOUP REQUIRED libsoup-2.4)
pkg_check_modules(PANGO REQUIRED pangocairo)
pkg_check_modules(HIDAPI REQUIRED hidapi-hidraw)

# Statically link
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static" CACHE STRING "Set C++ Compiler Flags" FORCE)

add_executable(chipotherm 
    button_actor.cpp
    clock_actor.cpp
    graph_actor.cpp
    io_control.cpp
    main.cpp
    monitor.cpp
    palette.cpp
    primary_scene.cpp
    qube_actor.cpp
    remote.cpp
    rounded_title_actor.cpp
    runloop.cpp
    utilities.cpp
)

target_include_directories(chipotherm PUBLIC
    ${SOUP_INCLUDE_DIRS}
    ${HIDAPI_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
    bubbles
)

target_link_libraries(chipotherm
    ${SOUP_LIBRARIES}
    ${PANGO_LIBRARIES}
    ${HIDAPI_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    bubbles
    tempered-static
)

