add_subdirectory(opengl)

set(INJECTORS_SRCS
    ${OPENGL_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/injectors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fontconfig_wrapper.c
    PARENT_SCOPE
)
