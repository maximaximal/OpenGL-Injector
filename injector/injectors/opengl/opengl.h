#pragma once

#include <EGL/egl.h>
#include <GL/glx.h>

#include <piga-injector_export.h>

void PIGA_INJECTOR_EXPORT glXSwapBuffers(Display *dpy, GLXDrawable drawable);

EGLBoolean PIGA_INJECTOR_EXPORT eglSwapBuffers(EGLDisplay display, EGLSurface surface);
