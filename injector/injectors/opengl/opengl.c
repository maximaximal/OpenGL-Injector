#include "../injectors.h"
#include "opengl.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void (*glXSwapBuffers_ptr_t)(Display *dpy, GLXDrawable);
typedef void* (*glXGetProcAddress_ptr_t)(const GLubyte *procName);

typedef void (*glGetIntegerv_ptr_t)(GLenum pname, GLint *v);
typedef void (*glGetFloatv_ptr_t)(GLenum pname, GLfloat *v);
typedef void (*glGetBooleanv_ptr_t)(GLenum pname, GLboolean *v);

typedef void (*glEnable_ptr_t)(GLenum pname);
typedef void (*glDisable_ptr_t)(GLenum pname);
typedef GLboolean (*glIsEnabled_ptr_t)(GLenum pname);
typedef GLboolean (*glIsEnabledi_ptr_t)(GLenum pname, GLuint index);

typedef void (*glActiveTexture_ptr_t)(GLenum texture);
typedef void (*glUseProgram_ptr_t)(GLuint program);
typedef void (*glBlendColor_ptr_t)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (*glFrontFace_ptr_t)(GLenum mode);
typedef void (*glColorMask_ptr_t)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (*glColorMaski_ptr_t)(GLuint buf, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (*glBlendFunc_ptr_t)(GLenum sfactor, GLenum dfactor);
typedef void (*glBindFramebuffer_ptr_t)(GLenum target, GLuint framebuffer);
typedef void (*glBindTexture_ptr_t)(GLenum target, GLuint texture);
typedef void (*glBindSampler_ptr_t)(GLuint unit, GLuint sampler);
typedef void (*glBindBuffer_ptr_t)(GLenum target, GLuint buffer);
typedef void (*glBindVertexArray_ptr_t)(GLuint array);
typedef void (*glPolygonMode_ptr_t)(GLenum face, GLenum mode);
typedef void (*glTexParameterf_ptr_t)(GLenum target, GLenum pname, GLfloat param);
typedef void (*glTexParameteri_ptr_t)(GLenum target, GLenum pname, GLint param);
typedef void (*glPixelStoref_ptr_t)(GLenum pname, GLfloat param);
typedef void (*glPixelStorei_ptr_t)(GLenum pname, GLint param);

void * opengl_glx_handle = 0;

glXSwapBuffers_ptr_t glXSwapBuffers_ptr = 0;
glXGetProcAddress_ptr_t glXGetProcAddress_ptr = 0;

glGetIntegerv_ptr_t glGetIntegerv_ptr = 0;
glGetBooleanv_ptr_t glGetBooleanv_ptr = 0;
glGetFloatv_ptr_t glGetFloatv_ptr = 0;

glEnable_ptr_t glEnable_ptr = 0;
glDisable_ptr_t glDisable_ptr = 0;
glIsEnabled_ptr_t glIsEnabled_ptr = 0;
glIsEnabledi_ptr_t glIsEnabledi_ptr = 0;

glActiveTexture_ptr_t glActiveTexture_ptr = 0;
glBindBuffer_ptr_t glBindBuffer_ptr = 0;
glBindFramebuffer_ptr_t glBindFramebuffer_ptr = 0;
glBindSampler_ptr_t glBindSampler_ptr = 0;
glBindTexture_ptr_t glBindTexture_ptr = 0;
glBindVertexArray_ptr_t glBindVertexArray_ptr = 0;
glBlendColor_ptr_t glBlendColor_ptr = 0;
glBlendFunc_ptr_t glBlendFunc_ptr = 0;
glColorMask_ptr_t glColorMask_ptr = 0;
glColorMaski_ptr_t glColorMaski_ptr = 0;
glFrontFace_ptr_t glFrontFace_ptr = 0;
glPixelStoref_ptr_t glPixelStoref_ptr = 0;
glPixelStorei_ptr_t glPixelStorei_ptr = 0;
glPolygonMode_ptr_t glPolygonMode_ptr = 0;
glTexParameterf_ptr_t glTexParameterf_ptr = 0;
glTexParameteri_ptr_t glTexParameteri_ptr = 0;
glUseProgram_ptr_t glUseProgram_ptr = 0;

struct GLStateBank
{
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

struct GLStateBank *stateBank = 0;

/** 
 * @brief Sets the state of the specified GL attribute, if the current value is different than the target.
 *
 * @param pname Name of the target attribute.
 * @param target Target value.
 * @param store Memory to store the original value in.
 */
inline void setGLAttr(GLenum pname, GLboolean target, GLboolean *store)
{
    *store = glIsEnabled_ptr(pname);
    if(target == GL_TRUE && *store == GL_FALSE)
        glEnable_ptr(pname);
    else if(target == GL_FALSE && *store == GL_TRUE)
        glDisable_ptr(pname);
}
inline void resetGLAttr(GLenum pname, GLboolean store)
{
    GLboolean active = glIsEnabled_ptr(pname);

    if(active != store) {
        if(store == GL_TRUE)
            glEnable_ptr(pname);
        else 
            glDisable_ptr(pname);
    }
}

void normalizeGLState()
{
    if(stateBank == 0) {
        stateBank = malloc(sizeof(struct GLStateBank));
    }
    
    // This code follows the example from:
    // https://github.com/nickguletskii/GLXOSD/blob/master/src/glxosd/rendering/normalise_gl_state.lua
    // It has been translated to C and extended to fit into this application.

    // Misc
    glGetIntegerv_ptr(GL_CURRENT_PROGRAM, &stateBank->currentProgram);
    glGetIntegerv_ptr(GL_FRONT_FACE, &stateBank->frontFace);
    glGetFloatv_ptr(GL_BLEND_COLOR, stateBank->blendColor);
    glGetBooleanv_ptr(GL_COLOR_WRITEMASK, &stateBank->colorWritemask);
    glGetIntegerv_ptr(GL_BLEND_SRC, &stateBank->blendSRC);
    glGetIntegerv_ptr(GL_BLEND_DST, &stateBank->blendDST);

    // Attributes
    setGLAttr(GL_BLEND, GL_TRUE, &stateBank->blend);
    setGLAttr(GL_COLOR_LOGIC_OP, GL_FALSE, &stateBank->colorLogic);
    setGLAttr(GL_CULL_FACE, GL_FALSE, &stateBank->cullFace);
    setGLAttr(GL_DEPTH_CLAMP, GL_FALSE, &stateBank->depthClamp);
    setGLAttr(GL_DEPTH_TEST, GL_FALSE, &stateBank->depthTest);
    setGLAttr(GL_DITHER, GL_FALSE, &stateBank->dither);
    setGLAttr(GL_FRAMEBUFFER_SRGB, GL_FALSE, &stateBank->framebufferSRGB);
    setGLAttr(GL_LINE_SMOOTH, GL_FALSE, &stateBank->lineSmooth);
    setGLAttr(GL_MULTISAMPLE, GL_FALSE, &stateBank->multisample);
    setGLAttr(GL_POLYGON_OFFSET_FILL, GL_FALSE, &stateBank->polygonOffsetFill);
    setGLAttr(GL_POLYGON_OFFSET_LINE, GL_FALSE, &stateBank->polygonOffsetLine);
    setGLAttr(GL_POLYGON_OFFSET_POINT, GL_FALSE, &stateBank->polygonOffsetPoint);
    setGLAttr(GL_POLYGON_SMOOTH, GL_FALSE, &stateBank->polygonSmooth);
    setGLAttr(GL_PRIMITIVE_RESTART, GL_FALSE, &stateBank->primitiveRestart);
    setGLAttr(GL_SAMPLE_ALPHA_TO_COVERAGE, GL_FALSE, &stateBank->sampleAlphaToCoverage);
    setGLAttr(GL_SAMPLE_ALPHA_TO_ONE, GL_FALSE, &stateBank->sampleAlphaToOne);
    setGLAttr(GL_SAMPLE_COVERAGE, GL_FALSE, &stateBank->sampleCoverage);
    setGLAttr(GL_SCISSOR_TEST, GL_FALSE, &stateBank->scissorTest);
    setGLAttr(GL_STENCIL_TEST, GL_FALSE, &stateBank->stencilTest);
    setGLAttr(GL_TEXTURE_CUBE_MAP_SEAMLESS, GL_FALSE, &stateBank->textureCubeMapSeamless);
    setGLAttr(GL_PROGRAM_POINT_SIZE, GL_FALSE, &stateBank->programPointSize);

    // Buffer State
    glGetIntegerv_ptr(GL_PIXEL_UNPACK_BUFFER_BINDING, &stateBank->pixelUnpackBufferBinding);
    glGetIntegerv_ptr(GL_ARRAY_BUFFER_BINDING, &stateBank->arrayBufferBinding);
    glGetIntegerv_ptr(GL_ACTIVE_TEXTURE, &stateBank->activeTexture);
    glGetIntegerv_ptr(GL_VERTEX_ARRAY_BINDING, &stateBank->vertexArrayBinding);
    glGetIntegerv_ptr(GL_ELEMENT_ARRAY_BUFFER_BINDING, &stateBank->elementArrayBufferBinding);
    glGetIntegerv_ptr(GL_READ_FRAMEBUFFER_BINDING, &stateBank->readFramebufferBinding);
    glGetIntegerv_ptr(GL_POLYGON_MODE, &stateBank->polygonMode);

    // Unpack States
    glGetIntegerv_ptr(GL_UNPACK_SWAP_BYTES, &stateBank->unpackSwapBuffers);
    glGetIntegerv_ptr(GL_UNPACK_LSB_FIRST, &stateBank->unpackLsbFirst);
    glGetIntegerv_ptr(GL_UNPACK_ROW_LENGTH, &stateBank->unpackRowLength);
    glGetIntegerv_ptr(GL_UNPACK_IMAGE_HEIGHT, &stateBank->unpackImageHeight);
    glGetIntegerv_ptr(GL_UNPACK_SKIP_ROWS, &stateBank->unpackSkipRows);
    glGetIntegerv_ptr(GL_UNPACK_SKIP_PIXELS, &stateBank->unpackSkipPixels);
    glGetIntegerv_ptr(GL_UNPACK_SKIP_IMAGES, &stateBank->unpackSkipImages);
    glGetIntegerv_ptr(GL_UNPACK_ALIGNMENT, &stateBank->unpackAlignment);

    // GLTEXTURE0
    glActiveTexture_ptr(GL_TEXTURE0);
    glGetIntegerv_ptr(GL_SAMPLER_BINDING, &stateBank->samplerBinding);
    glGetIntegerv_ptr(GL_TEXTURE_BINDING_2D, &stateBank->textureBinding2D);

    // Start Normalization.
    glUseProgram_ptr(0);
    glBlendColor_ptr(0, 0, 0, 0);
    glFrontFace_ptr(GL_CCW);
    glColorMask_ptr(1, 1, 1, 1);
    glBlendFunc_ptr(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindFramebuffer_ptr(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer_ptr(GL_READ_FRAMEBUFFER, 0);
    glPolygonMode_ptr(GL_FRONT_AND_BACK, GL_FILL);

    // This was a function call in the original code, but it was already made before,
    // so it is excluded here.
    // glActiveTexture_ptr(GL_TEXTURE0);

    glBindSampler_ptr(0, 0);
    glBindTexture_ptr(GL_TEXTURE_2D, 0);
	glTexParameteri_ptr(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri_ptr(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri_ptr(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri_ptr(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei_ptr(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei_ptr(GL_UNPACK_LSB_FIRST, GL_TRUE);
    glPixelStorei_ptr(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei_ptr(GL_UNPACK_IMAGE_HEIGHT, 0);
    glPixelStorei_ptr(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei_ptr(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei_ptr(GL_UNPACK_SKIP_IMAGES, 0);
    glPixelStorei_ptr(GL_UNPACK_ALIGNMENT, 4);
    glBindBuffer_ptr(GL_ARRAY_BUFFER, 0);
}
void resetGLState()
{
    // Misc
    glActiveTexture_ptr(GL_TEXTURE0);
    glBindSampler_ptr(0, stateBank->samplerBinding);
    glBindTexture_ptr(GL_TEXTURE_2D, stateBank->textureBinding2D);

    // Unpack States
	glPixelStorei_ptr(GL_UNPACK_SWAP_BYTES, stateBank->unpackSwapBuffers);
    glPixelStorei_ptr(GL_UNPACK_LSB_FIRST, stateBank->unpackLsbFirst);
    glPixelStorei_ptr(GL_UNPACK_ROW_LENGTH, stateBank->unpackRowLength);
    glPixelStorei_ptr(GL_UNPACK_IMAGE_HEIGHT, stateBank->unpackImageHeight);
    glPixelStorei_ptr(GL_UNPACK_SKIP_ROWS, stateBank->unpackSkipRows);
    glPixelStorei_ptr(GL_UNPACK_SKIP_PIXELS, stateBank->unpackSkipPixels);
    glPixelStorei_ptr(GL_UNPACK_SKIP_IMAGES, stateBank->unpackSkipImages);
    glPixelStorei_ptr(GL_UNPACK_ALIGNMENT, stateBank->unpackAlignment);

    // Buffer State
    glActiveTexture_ptr(stateBank->activeTexture);
	glBindVertexArray_ptr(stateBank->vertexArrayBinding);
	glBindBuffer_ptr(GL_PIXEL_UNPACK_BUFFER, stateBank->pixelUnpackBufferBinding);
	glBindBuffer_ptr(GL_ARRAY_BUFFER, stateBank->arrayBufferBinding);
	glBindBuffer_ptr(GL_ELEMENT_ARRAY_BUFFER, stateBank->elementArrayBufferBinding);
	glBindFramebuffer_ptr(GL_DRAW_FRAMEBUFFER, stateBank->drawFramebufferBinding);
    glBindFramebuffer_ptr(GL_READ_FRAMEBUFFER, stateBank->readFramebufferBinding);
    glPolygonMode_ptr(GL_FRONT_AND_BACK, stateBank->polygonMode);

    // Attributes
    resetGLAttr(GL_BLEND, stateBank->blend);
    resetGLAttr(GL_COLOR_LOGIC_OP, stateBank->colorLogic);
    resetGLAttr(GL_CULL_FACE, stateBank->cullFace);
    resetGLAttr(GL_DEPTH_CLAMP, stateBank->depthClamp);
    resetGLAttr(GL_DEPTH_TEST, stateBank->depthTest);
    resetGLAttr(GL_DITHER, stateBank->dither);
    resetGLAttr(GL_FRAMEBUFFER_SRGB, stateBank->framebufferSRGB);
    resetGLAttr(GL_LINE_SMOOTH, stateBank->lineSmooth);
    resetGLAttr(GL_MULTISAMPLE, stateBank->multisample);
    resetGLAttr(GL_POLYGON_OFFSET_FILL, stateBank->polygonOffsetFill);
    resetGLAttr(GL_POLYGON_OFFSET_LINE, stateBank->polygonOffsetLine);
    resetGLAttr(GL_POLYGON_OFFSET_POINT, stateBank->polygonOffsetPoint);
    resetGLAttr(GL_POLYGON_SMOOTH, stateBank->polygonSmooth);
    resetGLAttr(GL_PRIMITIVE_RESTART, stateBank->primitiveRestart);
    resetGLAttr(GL_SAMPLE_ALPHA_TO_COVERAGE, stateBank->sampleAlphaToCoverage);
    resetGLAttr(GL_SAMPLE_ALPHA_TO_ONE, stateBank->sampleAlphaToOne);
    resetGLAttr(GL_SAMPLE_COVERAGE, stateBank->sampleCoverage);
    resetGLAttr(GL_SCISSOR_TEST, stateBank->scissorTest);
    resetGLAttr(GL_STENCIL_TEST, stateBank->stencilTest);
    resetGLAttr(GL_TEXTURE_CUBE_MAP_SEAMLESS, stateBank->textureCubeMapSeamless);
    resetGLAttr(GL_PROGRAM_POINT_SIZE, stateBank->programPointSize);

    // Misc
    glBlendColor_ptr(stateBank->blendColor[0],
                     stateBank->blendColor[1],
                     stateBank->blendColor[2],
                     stateBank->blendColor[3]);
    glBlendFunc_ptr(stateBank->blendSRC, stateBank->blendDST);
    glFrontFace_ptr(stateBank->frontFace);
    glUseProgram_ptr(stateBank->currentProgram);
}

void glXSwapBuffers(Display *dpy, GLXDrawable drawable)
{
    struct piga_injector_handle_t* handle = piga_injector_init();

    if(handle->window_width == 0) {
        glXQueryDrawable(dpy, drawable, GLX_WIDTH, &handle->window_width);
    }
    if(handle->window_height == 0) {
        glXQueryDrawable(dpy, drawable, GLX_HEIGHT, &handle->window_height);
    }

    piga_injector_draw();

    // Receive the right glXSwapBuffers library.
    if(opengl_glx_handle == 0) {
        opengl_glx_handle = dlopen(handle->libGLX_path, RTLD_LAZY);
        printf("Opened real libGLX.\n");
    }

    if(glXSwapBuffers_ptr == 0) {
        glXSwapBuffers_ptr = dlsym(opengl_glx_handle, "glXSwapBuffers");
        glXGetProcAddress_ptr = dlsym(opengl_glx_handle, "glXGetProcAddress");
        printf("Read glXSwapBuffers and glXGetProcAddress ptr from libGLX.\n");
    }
    if(glGetBooleanv_ptr == 0) {
        // Receive all OpenGL functions.
        glGetBooleanv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetBooleanv");
        glGetIntegerv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetIntegerv");
        glGetFloatv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetFloatv");
        
        glActiveTexture_ptr = glXGetProcAddress_ptr((const unsigned char*) "glActiveTexture");
        glBindBuffer_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindBuffer");
        glBindFramebuffer_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindFramebuffer");
        glBindTexture_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindTexture");
        glBindVertexArray_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindVertexArray");
        glBlendColor_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBlendColor");
        glBlendFunc_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBlendFunc");
        glColorMask_ptr = glXGetProcAddress_ptr((const unsigned char*) "glColorMask");
        glColorMaski_ptr = glXGetProcAddress_ptr((const unsigned char*) "glColorMaski");
        glFrontFace_ptr = glXGetProcAddress_ptr((const unsigned char*) "glFrontFace");
        glPixelStorei_ptr = glXGetProcAddress_ptr((const unsigned char*) "glPixelStorei");
        glPixelStoref_ptr = glXGetProcAddress_ptr((const unsigned char*) "glPixelStoref");
        glPolygonMode_ptr = glXGetProcAddress_ptr((const unsigned char*) "glPolygonMode");
        glTexParameterf_ptr = glXGetProcAddress_ptr((const unsigned char*) "glTexParameterf");
        glTexParameteri_ptr = glXGetProcAddress_ptr((const unsigned char*) "glTexParameteri");
        glUseProgram_ptr = glXGetProcAddress_ptr((const unsigned char*) "glUseProgram");

        glEnable_ptr = glXGetProcAddress_ptr((const unsigned char*) "glEnable");
        glDisable_ptr = glXGetProcAddress_ptr((const unsigned char*) "glDisable");
        glIsEnabled_ptr = glXGetProcAddress_ptr((const unsigned char*) "glIsEnabled");
        glIsEnabledi_ptr = glXGetProcAddress_ptr((const unsigned char*) "glIsEnabledi");

        printf("Received OpenGL functions.\n");
    }

    // Draw the surface ontop of the drawable.
    // TODO
    // 1. Normalize context (Example: https://github.com/nickguletskii/GLXOSD/blob/master/src/glxosd/rendering/normalise_gl_state.lua)
    // 2. Render (Example: https://github.com/nickguletskii/GLXOSD/blob/master/src/glxosd/rendering/TextRenderer.lua)
    // 3. Test

    glXSwapBuffers_ptr(dpy, drawable);
}

EGLBoolean eglSwapBuffers(EGLDisplay display, EGLSurface surface)
{
    piga_injector_init();
}
