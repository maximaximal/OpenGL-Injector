#pragma once

#include "opengl.h"
#include <injector_export.h>
#include <stdbool.h>

INJECTOR_NO_EXPORT GLuint injector_make_shader(GLenum      type,
                                                       const char *path);
INJECTOR_NO_EXPORT GLuint injector_make_program(GLuint vertex_shader,
                                                        GLuint fragment_shader);
INJECTOR_NO_EXPORT bool injector_link_program(GLuint program);
