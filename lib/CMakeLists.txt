cmake_minimum_required(VERSION 3.2)
project(sdlpango C CXX)

set(SOURCE_FILES
    SDL_Pango/SDL_Pango.c
)

add_library(sdlpango SHARED STATIC ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DEPS_LIBRARIES})

# install(TARGETS SDL_Pango DESTINATION ${DIVISIBLE_INSTALL_LIB_DIR})
# install(FILES SDL_Pango.h DESTINATION ${DIVISIBLE_INSTALL_INCLUDE_DIR})

