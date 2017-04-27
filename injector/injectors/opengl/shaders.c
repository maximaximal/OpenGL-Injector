#include "shaders.h"
#include "../injectors.h"
#include <stdlib.h>
#include <stdio.h>

GLuint piga_opengl_make_shader(GLenum type, const char *path)
{
    GLint glLength;
    size_t length;
    GLchar *source = piga_read_file(path, &length);
    glLength = length; // Explicit downcasting.
    GLuint shader;
    GLint ok;

    // File could not be read.
    if(!source) {
        printf("No shader \"%s\" exists!\n", path);
        return 0;
    }

    shader = glCreateShader_ptr(type);
    glShaderSource_ptr(shader, 1, (const GLchar**) &source, &glLength);
    glCompileShader_ptr(shader);
    glGetShaderiv_ptr(shader, GL_COMPILE_STATUS, &ok);

    printf("ADD SHADER: \n%s!\n", source);

    free(source);

    if (!ok) {
        printf("Could not compile the shader %s!\n", path);
        return 0;
    }
    return shader;
}
GLuint piga_opengl_make_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint ok;
    GLuint program = glCreateProgram_ptr();
    glAttachShader_ptr(program, vertex_shader);
    glAttachShader_ptr(program, fragment_shader);
    glLinkProgram_ptr(program);
    glGetProgramiv_ptr(program, GL_LINK_STATUS, &ok);
    if(!ok) {
        printf("Could not link program!\n");
        return 0;
    }
    return program;
}
