add_subdirectory(opengl)

set(INJECTORS_SRCS
    ${OPENGL_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/injectors.c
    PARENT_SCOPE
)
