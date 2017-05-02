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
    glShaderSource_ptr(shader, 1, (const GLchar**) &source, 0);
    glCompileShader_ptr(shader);
    glGetShaderiv_ptr(shader, GL_COMPILE_STATUS, &ok);

    printf("ADD SHADER: \n%s!\n", source);

    free(source);

    // Debug output.
    char buffer[512];
    glGetShaderInfoLog_ptr(shader, sizeof(buffer), NULL, buffer);
    printf("Shader-Info-Log: %s\n", buffer);

    if (!ok) {
        printf("Could not compile the shader %s!\n", path);
        return 0;
    }
    
    return shader;
}
GLuint piga_opengl_make_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint program = glCreateProgram_ptr();
    glAttachShader_ptr(program, vertex_shader);
    glAttachShader_ptr(program, fragment_shader);
    return program;
}
bool piga_opengl_link_program(GLuint program)
{
    GLint ok;
    glLinkProgram_ptr(program);

    // Debug output.
    char buffer[512];
    glGetProgramInfoLog_ptr(program, sizeof(buffer), NULL, buffer);
    printf("Program-Info-Log: %s\n", buffer);

    glGetProgramiv_ptr(program, GL_LINK_STATUS, &ok);
    if(!ok) {
        printf("Could not link program!\n");
        return false;
    }
    return true;
}
