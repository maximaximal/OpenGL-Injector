#include "../injectors.h"
#include "opengl.h"
#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

static const GLfloat squareVertices[] = {
    -1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f,1.0f,

    -1.0f,-1.0f,
    1.0f,1.0f,
    1.0f,-1.0f
};

static const GLfloat textureVertices[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

#define CHECK_GL_FUNC_PTR(PTR) if( PTR == 0 ) printf("The function " #PTR " could not be read!\n");

/** 
 * @brief Sets the state of the specified GL attribute, if the current value is different than the target.
 *
 * @param pname Name of the target attribute.
 * @param target Target value.
 * @param store Memory to store the original value in.
 */
inline static void setGLAttr(GLenum pname, GLboolean target, GLboolean *store)
{
    *store = glIsEnabled_ptr(pname);
    if(target == GL_TRUE && *store == GL_FALSE)
        glEnable_ptr(pname);
    else if(target == GL_FALSE && *store == GL_TRUE)
        glDisable_ptr(pname);
}
inline static void resetGLAttr(GLenum pname, GLboolean store)
{
    GLboolean active = glIsEnabled_ptr(pname);

    if(active != store) {
        if(store == GL_TRUE)
            glEnable_ptr(pname);
        else 
            glDisable_ptr(pname);
    }
}

static void normalizeGLState()
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
    glActiveTexture_ptr(GL_TEXTURE0);

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
static void resetGLState()
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
    //glUseProgram_ptr(stateBank->currentProgram);
    glBlendColor_ptr(stateBank->blendColor[0],
                     stateBank->blendColor[1],
                     stateBank->blendColor[2],
                     stateBank->blendColor[3]);
    glBlendFunc_ptr(stateBank->blendSRC, stateBank->blendDST);
    glFrontFace_ptr(stateBank->frontFace);
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

    // Receive the right glXSwapBuffers library.
    if(opengl_glx_handle == 0) {
        opengl_glx_handle = dlopen(handle->libGLX_path, RTLD_LAZY);
        printf("Opened real libGLX.\n");
    }

    if(glXSwapBuffers_ptr == 0) {
        glXSwapBuffers_ptr = dlsym(opengl_glx_handle, "glXSwapBuffers");
        glXGetProcAddress_ptr = dlsym(opengl_glx_handle, "glXGetProcAddress");
        glXGetCurrentContext_ptr = dlsym(opengl_glx_handle, "glXGetCurrentContext");
        printf("Read glXSwapBuffers and glXGetProcAddress ptr from libGLX.\n");
    }

    if(glGetBooleanv_ptr == 0) {
        // Reset GL variables.
        gl_vertex_shader = 0;
        gl_fragment_shader = 0;
        gl_program = 0;
        gl_position = 0;
        gl_texture = 0;
        gl_texture_uniform_texture = 0;
        gl_texture_uniform_texcoord = 0;
        gl_vbo[0] = 0;
        gl_vbo[1] = 0;
        gl_vao = 0;
        stateBank = 0;

        gl_shader_initialized = false;
        gl_program_initialized = false;
        gl_all_initialized = false;
        
        // Receive all OpenGL functions.
        glGetBooleanv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetBooleanv");
        CHECK_GL_FUNC_PTR(glGetBooleanv_ptr)
        glGetIntegerv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetIntegerv");
        CHECK_GL_FUNC_PTR(glGetIntegerv_ptr)
        glGetFloatv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetFloatv");
        CHECK_GL_FUNC_PTR(glGetFloatv_ptr)
        glGetShaderInfoLog_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetShaderInfoLog");
        glGetProgramInfoLog_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetProgramInfoLog");
        CHECK_GL_FUNC_PTR(glGetProgramInfoLog_ptr)
        glGetError_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetError");
        CHECK_GL_FUNC_PTR(glGetError_ptr)
        
        glActiveTexture_ptr = glXGetProcAddress_ptr((const unsigned char*) "glActiveTexture");
        glBindBuffer_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindBuffer");
        glBindFramebuffer_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindFramebuffer");
        glBindTexture_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindTexture");
        glBindVertexArray_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindVertexArray");
        glBindSampler_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindSampler");
        glBindAttribLocation_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBindAttribLocation");
        CHECK_GL_FUNC_PTR(glBindAttribLocation_ptr)
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

        glCreateShader_ptr = glXGetProcAddress_ptr((const unsigned char*) "glCreateShader");
        CHECK_GL_FUNC_PTR(glCreateShader_ptr)
        glShaderSource_ptr = glXGetProcAddress_ptr((const unsigned char*) "glShaderSource");
        CHECK_GL_FUNC_PTR(glShaderSource_ptr)
        glCompileShader_ptr = glXGetProcAddress_ptr((const unsigned char*) "glCompileShader");
        CHECK_GL_FUNC_PTR(glCompileShader_ptr)
        glDeleteShader_ptr = glXGetProcAddress_ptr((const unsigned char*) "glDeleteShader");
        CHECK_GL_FUNC_PTR(glDeleteShader_ptr)
        glCreateProgram_ptr = glXGetProcAddress_ptr((const unsigned char*) "glCreateProgram");
        CHECK_GL_FUNC_PTR(glCreateProgram_ptr)
        glAttachShader_ptr = glXGetProcAddress_ptr((const unsigned char*) "glAttachShader");
        CHECK_GL_FUNC_PTR(glAttachShader_ptr)
        glLinkProgram_ptr = glXGetProcAddress_ptr((const unsigned char*) "glLinkProgram");
        CHECK_GL_FUNC_PTR(glLinkProgram_ptr)
        glGetProgramiv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetProgramiv");
        CHECK_GL_FUNC_PTR(glGetProgramiv_ptr)
        glGetShaderiv_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetShaderiv");
        CHECK_GL_FUNC_PTR(glGetShaderiv_ptr)
        glDeleteProgram_ptr = glXGetProcAddress_ptr((const unsigned char*) "glDeleteProgram");
        CHECK_GL_FUNC_PTR(glDeleteProgram_ptr)
        glGetAttribLocation_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetAttribLocation");
        CHECK_GL_FUNC_PTR(glGetAttribLocation_ptr)
        glGetUniformLocation_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGetUniformLocation");
        CHECK_GL_FUNC_PTR(glGetUniformLocation_ptr)
        glGenTextures_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGenTextures");
        CHECK_GL_FUNC_PTR(glGenTextures_ptr)
        glGenBuffers_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGenBuffers");
        CHECK_GL_FUNC_PTR(glGenBuffers_ptr)
        glGenVertexArrays_ptr = glXGetProcAddress_ptr((const unsigned char*) "glGenVertexArrays");
        CHECK_GL_FUNC_PTR(glGenVertexArrays_ptr)
        glBufferData_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBufferData");
        CHECK_GL_FUNC_PTR(glBufferData_ptr)
        glBufferSubData_ptr = glXGetProcAddress_ptr((const unsigned char*) "glBufferSubData");
        CHECK_GL_FUNC_PTR(glBufferSubData_ptr)
        glTexImage2D_ptr = glXGetProcAddress_ptr((const unsigned char*) "glTexImage2D");
        CHECK_GL_FUNC_PTR(glTexImage2D_ptr)
        glUniform1i_ptr = glXGetProcAddress_ptr((const unsigned char*) "glUniform1i");
        CHECK_GL_FUNC_PTR(glUniform1i_ptr)
        glVertexAttribPointer_ptr = glXGetProcAddress_ptr((const unsigned char*) "glVertexAttribPointer");
        CHECK_GL_FUNC_PTR(glVertexAttribPointer_ptr)
        glEnableVertexAttribArray_ptr = glXGetProcAddress_ptr((const unsigned char*) "glEnableVertexAttribArray");
        CHECK_GL_FUNC_PTR(glVertexAttribPointer_ptr)

        printf("Received OpenGL functions.\n");
    }

    normalizeGLState();
    piga_opengl_check_for_errors("Normalize GL-State");
 
    if(!gl_shader_initialized) {
        // Check the version.
        GLint major, minor;
        glGetIntegerv_ptr(GL_MAJOR_VERSION, &major);
        glGetIntegerv_ptr(GL_MINOR_VERSION, &minor);
        printf("GL-Version: %i.%i\n", major, minor);
        
        gl_vertex_shader = piga_opengl_make_shader(GL_VERTEX_SHADER,
                                                   handle->vertex_shader);
        piga_opengl_check_for_errors("Make Vertex Shader");

        gl_fragment_shader = piga_opengl_make_shader(GL_FRAGMENT_SHADER,
                                                   handle->fragment_shader);
        piga_opengl_check_for_errors("Make Fragment Shader");

        gl_shader_initialized = true;
    }

    if(gl_shader_initialized && !gl_program_initialized) {
        gl_program = piga_opengl_make_program(gl_vertex_shader, gl_fragment_shader);
        piga_opengl_check_for_errors("Make Program");

        if(!piga_opengl_link_program(gl_program)) {
            printf("Could not link program!");
        } else {
            printf("Read shaders and linked program (%i).\n", gl_program);
        }
        piga_opengl_check_for_errors("Link Program");
        
        gl_program_initialized = true;
    }

    if(!gl_all_initialized) {
        // Assign a texture.
        glGenTextures_ptr(1, &gl_texture);
        piga_opengl_check_for_errors("Gen-Texture");
        glBindTexture_ptr(GL_TEXTURE_RECTANGLE, gl_texture);
        piga_opengl_check_for_errors("Bind-Texture");

        glTexParameteri_ptr(GL_TEXTURE_RECTANGLE, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri_ptr(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri_ptr(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri_ptr(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        piga_opengl_check_for_errors("Texture Properties");
        
        glBindTexture_ptr(GL_TEXTURE_RECTANGLE, 0);
        piga_opengl_check_for_errors("Un-Bind Texture");

        // Generate the VAO.
        glGenVertexArrays_ptr(1, &gl_vao);
        piga_opengl_check_for_errors("Generate VAO.");
        glBindVertexArray_ptr(gl_vao);
        piga_opengl_check_for_errors("Bind VAO.");
        
        // Generate the VBOs.
        glGenBuffers_ptr(2, gl_vbo);
        piga_opengl_check_for_errors("Generate 2 VBOs");

        // Bind the square.
        glBindBuffer_ptr(GL_ARRAY_BUFFER, gl_vbo[0]);
        piga_opengl_check_for_errors("Bind vertices buffer.");
        glBufferData_ptr(GL_ARRAY_BUFFER, sizeof(squareVertices),
                         squareVertices, GL_STATIC_DRAW);
        piga_opengl_check_for_errors("Fill vertices buffer.");
        glVertexAttribPointer_ptr(glGetAttribLocation_ptr(gl_program, "position"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        GLint position = glGetAttribLocation_ptr(gl_program, "position");

        if(position != -1) {
            piga_opengl_check_for_errors("Define attrib for vertices buffer.");
            glEnableVertexAttribArray_ptr(glGetAttribLocation_ptr(gl_program, "position"));
            piga_opengl_check_for_errors("Enable attrib for vertices buffer.");
            glBindBuffer_ptr(GL_ARRAY_BUFFER, gl_vbo[0]);
            piga_opengl_check_for_errors("Re-Bind vertices buffer.");
        } else {
            printf("No position attribute found!\n");
        }

        // Bind the texture coords.
        glBindBuffer_ptr(GL_ARRAY_BUFFER, gl_vbo[1]);
        piga_opengl_check_for_errors("Bind texcoords buffer.");
        glBufferData_ptr(GL_ARRAY_BUFFER, sizeof(textureVertices),
                         textureVertices, GL_STATIC_DRAW);
        piga_opengl_check_for_errors("Fill texcoord buffer.");

        GLint texcoord = glGetAttribLocation_ptr(gl_program, "texcoord");
        if(texcoord != -1) {
            glVertexAttribPointer_ptr(texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
            piga_opengl_check_for_errors("Define attrib texcoords buffer.");
            glEnableVertexAttribArray_ptr(texcoord);
            piga_opengl_check_for_errors("Enable attrib texcoords buffer.");
            glBindBuffer_ptr(GL_ARRAY_BUFFER, gl_vbo[1]);
            piga_opengl_check_for_errors("Re-Bind texcoords buffer.");
        } else {
            printf("No texcoord attribute found!\n");
        }
        

        glBindVertexArray_ptr(0);
        piga_opengl_check_for_errors("Un-Bind VAO");

        gl_all_initialized = true;
    }

    piga_injector_draw();
    piga_opengl_check_for_errors("Piga Injector Draw");

    if(handle->draw_request) {
        // Upload the cairo surface to opengl.
        if(handle->cairo_surface == 0) {
            printf("No cairo surface available!\n");
            goto exit_return;
        }
        
        glUseProgram_ptr(gl_program);
        piga_opengl_check_for_errors("Use Program (Draw)");
        glBindVertexArray_ptr(gl_vao);
        piga_opengl_check_for_errors("Bind VAO (Draw)");

        // Update the texture.
        glBindTexture_ptr(GL_TEXTURE_RECTANGLE, gl_texture);
        piga_opengl_check_for_errors("Bind Texture (Draw)");
        glActiveTexture_ptr(GL_TEXTURE0);
        piga_opengl_check_for_errors("Active Texture (Draw)");

        glTexImage2D_ptr(GL_TEXTURE_RECTANGLE, 0, GL_RGBA,
                         handle->window_width, handle->window_height,
                         0, GL_BGRA, GL_UNSIGNED_BYTE, handle->cairo_data);
        piga_opengl_check_for_errors("Fill Texture (Draw)");

        // A very good example (ES 2.0) is here: http://stackoverflow.com/a/4227878
        glDrawArrays(GL_TRIANGLES, 0, 6);
        piga_opengl_check_for_errors("Draw Arrays");

        glUseProgram_ptr(0);
        glBindVertexArray_ptr(0);
        glBindBuffer_ptr(GL_ARRAY_BUFFER, 0);
        glBindTexture_ptr(GL_TEXTURE_2D, 0);
    }
    
exit_return:
    glUseProgram_ptr(0);
    piga_opengl_check_for_errors("Cleanup: Use Program 0");
    resetGLState();
    piga_opengl_check_for_errors("Cleanup: Reset GL-State");

    glXSwapBuffers_ptr(dpy, drawable);
    piga_opengl_check_for_errors("GLX-SWAP BUFFERS");
}

EGLBoolean eglSwapBuffers(EGLDisplay display, EGLSurface surface)
{
    piga_injector_init();
}

PIGA_INJECTOR_EXPORT bool piga_opengl_check_for_errors(const char *msg)
{
    if(msg == 0)
        msg = "(No Message)";
    
    GLenum error = glGetError_ptr();
    if(error != GL_NO_ERROR) {
        switch(error) {
        case GL_INVALID_ENUM:
            printf("[%s] OpenGL-Error: Invalid enum.\n", msg);
            break;
        case GL_INVALID_VALUE:
            printf("[%s] OpenGL-Error: Invalid value.\n", msg);
            break;
        case GL_INVALID_OPERATION:
            printf("[%s] OpenGL-Error: Invalid operation.\n", msg);
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("[%s] OpenGL-Error: Invalid framebuffer operation.\n", msg);
            break;
        case GL_OUT_OF_MEMORY:
            printf("[%s] OpenGL-Error: Out of memory.\n", msg);
            break;
        default:
            printf("[%s] OpenGL-Error: Unknown Error!\n", msg);
        }
        return true;
    }
    return false;
}
