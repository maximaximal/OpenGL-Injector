#pragma once

#include "../injector_helpers.h"

#include <X11/Xlib.h>

/*
 * Attribution for the hooks below primarily goes to nickguletskii in his
 * project GLXOSD.
 * Here is the original source, which was adapted to fit into this project:
 * https://github.com/nickguletskii/GLXOSD/blob/master/src/glinject/x_events.c
 * (MIT-License)
 */

INJECTOR_OVERRIDE_SYMBOL(XNextEvent, int, (Display * dpy, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XPeekEvent, int, (Display * dpy, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XWindowEvent,
                         int,
                         (Display * dpy, Window w, long mask, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XCheckWindowEvent,
                         Bool,
                         (Display * dpy, Window w, long mask, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XMaskEvent,
                         int,
                         (Display * dpy, long mask, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XCheckMaskEvent,
                         Bool,
                         (Display * dpy, long mask, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XCheckTypedEvent,
                         Bool,
                         (Display * dpy, int type, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XCheckTypedWindowEvent,
                         Bool,
                         (Display * dpy, Window w, int type, XEvent *event));

INJECTOR_OVERRIDE_SYMBOL(XIfEvent,
                         int,
                         (Display * dpy,
                          XEvent *event,
                          Bool (*predicate)(Display *, XEvent *, XPointer),
                          XPointer arg));

INJECTOR_OVERRIDE_SYMBOL(XCheckIfEvent,
                         Bool,
                         (Display * dpy,
                          XEvent *event,
                          Bool (*predicate)(Display *, XEvent *, XPointer),
                          XPointer arg));

INJECTOR_OVERRIDE_SYMBOL(XPeekIfEvent,
                         int,
                         (Display * dpy,
                          XEvent *event,
                          Bool (*predicate)(Display *, XEvent *, XPointer),
                          XPointer arg));

INJECTOR_OVERRIDE_SYMBOL(XPending, int, (Display * dpy));

void *                  injector_x_events_handle;
INJECTOR_NO_EXPORT void injector_load_x_events(const char *path);
