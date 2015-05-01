include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${SDL_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(spaaace
    space.cpp
    Star.h Star.cpp
)

target_link_libraries(spaaace
    ${SDL_LIBRARY}
    ${OPENGL_LIBRARIES}
)
