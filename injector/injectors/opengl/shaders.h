#pragma once

#include "opengl.h"
#include <piga-injector_export.h>

PIGA_INJECTOR_NO_EXPORT GLuint piga_opengl_make_shader(GLenum type, const char *path);
PIGA_INJECTOR_NO_EXPORT GLuint piga_opengl_make_program(GLuint vertex_shader, GLuint fragment_shader);
