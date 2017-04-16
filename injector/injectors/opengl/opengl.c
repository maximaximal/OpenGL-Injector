#include "../injectors.h"
#include "opengl.h"

#include <stdio.h>

void glXSwapBuffers(Display *dpy, GLXDrawable drawable)
{
    struct piga_injector_handle_t* handle = piga_injector_init();
}

EGLBoolean eglSwapBuffers(EGLDisplay display, EGLSurface surface)
{
    piga_injector_init();
}
