add_subdirectory(opengl)
add_subdirectory(x11)
add_subdirectory(xkbcomp)

set(INJECTORS_SRCS
    ${OPENGL_SRCS}
    ${X11_SRCS}
    ${XKBCOMP_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/injectors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fontconfig_wrapper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xkb_parser.c
    PARENT_SCOPE
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
