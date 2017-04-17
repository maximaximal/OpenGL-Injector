#include "../injectors.h"
#include "opengl.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void (*glXSwapBuffers_ptr_t)(Display *dpy, GLXDrawable);

void * opengl_glx_handle = 0;

glXSwapBuffers_ptr_t glXSwapBuffers_ptr = 0;

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
	printf("Read glXSwapBuffers ptr from libGLX.\n");
    }

    // Draw the surface ontop of the drawable.
    

    glXSwapBuffers_ptr(dpy, drawable);
}

EGLBoolean eglSwapBuffers(EGLDisplay display, EGLSurface surface)
{
    piga_injector_init();
}
