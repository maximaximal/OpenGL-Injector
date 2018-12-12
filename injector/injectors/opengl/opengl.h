#pragma once

#include <EGL/egl.h>
#include <GL/glx.h>
#include <stdbool.h>

#include <injector_export.h>

typedef void (*glXSwapBuffers_ptr_t)(Display *dpy, GLXDrawable);
typedef void *(*glXGetProcAddress_ptr_t)(const GLubyte *procName);
typedef GLXContext (*glXGetCurrentContext_ptr_t)();

typedef void (*glGetIntegerv_ptr_t)(GLenum pname, GLint *v);
typedef void (*glGetFloatv_ptr_t)(GLenum pname, GLfloat *v);
typedef void (*glGetBooleanv_ptr_t)(GLenum pname, GLboolean *v);
typedef void (*glGetShaderInfoLog_ptr_t)(GLuint   shader,
                                         GLsizei  maxLength,
                                         GLsizei *length,
                                         GLchar * infoLog);
typedef void (*glGetProgramInfoLog_ptr_t)(GLuint   shader,
                                          GLsizei  maxLength,
                                          GLsizei *length,
                                          GLchar * infoLog);
typedef GLenum (*glGetError_ptr_t)();

typedef void (*glEnable_ptr_t)(GLenum pname);
typedef void (*glDisable_ptr_t)(GLenum pname);
typedef GLboolean (*glIsEnabled_ptr_t)(GLenum pname);
typedef GLboolean (*glIsEnabledi_ptr_t)(GLenum pname, GLuint index);

typedef void (*glActiveTexture_ptr_t)(GLenum texture);
typedef void (*glUseProgram_ptr_t)(GLuint program);
typedef void (*glBlendColor_ptr_t)(GLclampf red,
                                   GLclampf green,
                                   GLclampf blue,
                                   GLclampf alpha);
typedef void (*glFrontFace_ptr_t)(GLenum mode);
typedef void (*glColorMask_ptr_t)(GLboolean red,
                                  GLboolean green,
                                  GLboolean blue,
                                  GLboolean alpha);
typedef void (*glColorMaski_ptr_t)(GLuint    buf,
                                   GLboolean red,
                                   GLboolean green,
                                   GLboolean blue,
                                   GLboolean alpha);
typedef void (*glBlendFunc_ptr_t)(GLenum sfactor, GLenum dfactor);
typedef void (*glBindFramebuffer_ptr_t)(GLenum target, GLuint framebuffer);
typedef void (*glBindTexture_ptr_t)(GLenum target, GLuint texture);
typedef void (*glBindSampler_ptr_t)(GLuint unit, GLuint sampler);
typedef void (*glBindBuffer_ptr_t)(GLenum target, GLuint buffer);
typedef void (*glBindVertexArray_ptr_t)(GLuint array);
typedef void (*glBindAttribLocation_ptr_t)(GLuint        program,
                                           GLuint        index,
                                           const GLchar *name);

typedef void (*glPolygonMode_ptr_t)(GLenum face, GLenum mode);
typedef void (*glTexParameterf_ptr_t)(GLenum  target,
                                      GLenum  pname,
                                      GLfloat param);
typedef void (*glTexParameteri_ptr_t)(GLenum target, GLenum pname, GLint param);
typedef void (*glPixelStoref_ptr_t)(GLenum pname, GLfloat param);
typedef void (*glPixelStorei_ptr_t)(GLenum pname, GLint param);

typedef GLuint (*glCreateShader_ptr_t)(GLenum shaderType);
typedef void (*glShaderSource_ptr_t)(GLuint         shader,
                                     GLsizei        count,
                                     const GLchar **string,
                                     const GLint *  length);
typedef void (*glCompileShader_ptr_t)(GLuint shader);
typedef void (*glGetShaderiv_ptr_t)(GLuint shader, GLenum pname, GLint *params);
typedef void (*glDeleteShader_ptr_t)(GLuint shader);
typedef GLuint (*glCreateProgram_ptr_t)();
typedef void (*glAttachShader_ptr_t)(GLuint program, GLuint shader);
typedef void (*glLinkProgram_ptr_t)(GLuint program);
typedef void (*glGetProgramiv_ptr_t)(GLuint program,
                                     GLenum pname,
                                     GLint *params);
typedef void (*glDeleteProgram_ptr_t)(GLuint program);
typedef GLint (*glGetAttribLocation_ptr_t)(GLuint program, const GLchar *name);
typedef GLint (*glGetUniformLocation_ptr_t)(GLuint program, const GLchar *name);
typedef void (*glGenTextures_ptr_t)(GLsizei size, GLuint *textures);
typedef void (*glGenBuffers_ptr_t)(GLsizei size, GLuint *buffers);
typedef void (*glGenVertexArrays_ptr_t)(GLsizei size, GLuint *vaos);
typedef void (*glBufferData_ptr_t)(GLenum        target,
                                   GLsizei       size,
                                   const GLvoid *data,
                                   GLenum        usage);
typedef void (*glBufferSubData_ptr_t)(GLenum        target,
                                      GLintptr      offset,
                                      GLsizeiptr    size,
                                      const GLvoid *data);
typedef void (*glTexImage2D_ptr_t)(GLenum        target,
                                   GLint         level,
                                   GLint         internalFormat,
                                   GLsizei       width,
                                   GLsizei       height,
                                   GLint         border,
                                   GLenum        format,
                                   GLenum        type,
                                   const GLvoid *data);
typedef void (*glTexSubImage2D_ptr_t)(GLenum        target,
                                      GLint         level,
                                      GLsizei       x,
                                      GLsizei       y,
                                      GLsizei       width,
                                      GLsizei       height,
                                      GLenum        format,
                                      GLenum        type,
                                      const GLvoid *data);
typedef void (*glUniform1i_ptr_t)(GLint location, GLint v0);
typedef void (*glVertexAttribPointer_ptr_t)(GLuint        index,
                                            GLint         size,
                                            GLenum        type,
                                            GLboolean     normalized,
                                            GLsizei       stride,
                                            const GLvoid *pointer);
typedef void (*glEnableVertexAttribArray_ptr_t)(GLuint index);

void *injector_glx_handle;

glXSwapBuffers_ptr_t       glXSwapBuffers_ptr;
glXGetProcAddress_ptr_t    glXGetProcAddress_ptr;
glXGetCurrentContext_ptr_t glXGetCurrentContext_ptr;

glGetIntegerv_ptr_t       glGetIntegerv_ptr;
glGetBooleanv_ptr_t       glGetBooleanv_ptr;
glGetFloatv_ptr_t         glGetFloatv_ptr;
glGetShaderInfoLog_ptr_t  glGetShaderInfoLog_ptr;
glGetProgramInfoLog_ptr_t glGetProgramInfoLog_ptr;
glGetError_ptr_t          glGetError_ptr;

glEnable_ptr_t     glEnable_ptr;
glDisable_ptr_t    glDisable_ptr;
glIsEnabled_ptr_t  glIsEnabled_ptr;
glIsEnabledi_ptr_t glIsEnabledi_ptr;

glActiveTexture_ptr_t      glActiveTexture_ptr;
glBindBuffer_ptr_t         glBindBuffer_ptr;
glBindFramebuffer_ptr_t    glBindFramebuffer_ptr;
glBindSampler_ptr_t        glBindSampler_ptr;
glBindTexture_ptr_t        glBindTexture_ptr;
glBindVertexArray_ptr_t    glBindVertexArray_ptr;
glBindAttribLocation_ptr_t glBindAttribLocation_ptr;
glBlendColor_ptr_t         glBlendColor_ptr;
glBlendFunc_ptr_t          glBlendFunc_ptr;
glColorMask_ptr_t          glColorMask_ptr;
glColorMaski_ptr_t         glColorMaski_ptr;
glFrontFace_ptr_t          glFrontFace_ptr;
glPixelStoref_ptr_t        glPixelStoref_ptr;
glPixelStorei_ptr_t        glPixelStorei_ptr;
glPolygonMode_ptr_t        glPolygonMode_ptr;
glTexParameterf_ptr_t      glTexParameterf_ptr;
glTexParameteri_ptr_t      glTexParameteri_ptr;
glUseProgram_ptr_t         glUseProgram_ptr;

glCreateShader_ptr_t            glCreateShader_ptr;
glShaderSource_ptr_t            glShaderSource_ptr;
glCompileShader_ptr_t           glCompileShader_ptr;
glDeleteShader_ptr_t            glDeleteShader_ptr;
glCreateProgram_ptr_t           glCreateProgram_ptr;
glAttachShader_ptr_t            glAttachShader_ptr;
glLinkProgram_ptr_t             glLinkProgram_ptr;
glGetProgramiv_ptr_t            glGetProgramiv_ptr;
glGetShaderiv_ptr_t             glGetShaderiv_ptr;
glDeleteProgram_ptr_t           glDeleteProgram_ptr;
glGetAttribLocation_ptr_t       glGetAttribLocation_ptr;
glGetUniformLocation_ptr_t      glGetUniformLocation_ptr;
glGenTextures_ptr_t             glGenTextures_ptr;
glGenBuffers_ptr_t              glGenBuffers_ptr;
glGenVertexArrays_ptr_t         glGenVertexArrays_ptr;
glBufferData_ptr_t              glBufferData_ptr;
glBufferSubData_ptr_t           glBufferSubData_ptr;
glTexImage2D_ptr_t              glTexImage2D_ptr;
glTexSubImage2D_ptr_t           glTexSubImage2D_ptr;
glUniform1i_ptr_t               glUniform1i_ptr;
glVertexAttribPointer_ptr_t     glVertexAttribPointer_ptr;
glEnableVertexAttribArray_ptr_t glEnableVertexAttribArray_ptr;

bool   gl_shader_initialized;
bool   gl_program_initialized;
bool   gl_all_initialized;
GLuint gl_vertex_shader;
GLuint gl_fragment_shader;
GLuint gl_program;
GLuint gl_texture;
GLint  gl_position;
GLint  gl_texture_uniform_texture;
GLint  gl_texture_uniform_texcoord;
GLuint gl_vbo[2];
GLuint gl_vao;

struct GLStateBank {
    GLboolean blend;
    GLboolean colorLogic;
    GLboolean cullFace;
    GLboolean depthClamp;
    GLboolean depthTest;
    GLboolean dither;
    GLboolean framebufferSRGB;
    GLboolean lineSmooth;
    GLboolean multisample;
    GLboolean polygonOffsetFill;

    GLboolean polygonOffsetLine;
    GLboolean polygonOffsetPoint;
    GLboolean polygonSmooth;
    GLboolean primitiveRestart;
    GLboolean sampleAlphaToCoverage;
    GLboolean sampleAlphaToOne;
    GLboolean sampleCoverage;
    GLboolean scissorTest;
    GLboolean stencilTest;
    GLboolean textureCubeMapSeamless;

    GLboolean programPointSize;
    GLboolean colorWritemask;

    GLint currentProgram;
    GLint frontFace;
    GLint blendSRC;
    GLint blendDST;

    GLint pixelUnpackBufferBinding;
    GLint arrayBufferBinding;
    GLint activeTexture;
    GLint vertexArrayShading;
    GLint vertexArrayBinding;
    GLint elementArrayBufferBinding;
    GLint drawFramebufferBinding;
    GLint readFramebufferBinding;
    GLint polygonMode;

    GLint unpackSwapBuffers;
    GLint unpackLsbFirst;
    GLint unpackRowLength;
    GLint unpackImageHeight;
    GLint unpackSkipRows;
    GLint unpackSkipPixels;
    GLint unpackSkipImages;
    GLint unpackAlignment;

    GLint samplerBinding;
    GLint textureBinding2D;

    GLfloat blendColor[4];
};

struct GLStateBank *stateBank;

void INJECTOR_EXPORT glXSwapBuffers(Display *dpy, GLXDrawable drawable);

EGLBoolean INJECTOR_EXPORT eglSwapBuffers(EGLDisplay display,
                                          EGLSurface surface);

INJECTOR_EXPORT bool injector_check_for_errors();
