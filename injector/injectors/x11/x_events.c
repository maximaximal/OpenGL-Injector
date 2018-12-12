#include "x_events.h"

#include "../injectors.h"
#include "x_handling.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(XNextEvent,
                                    int,
                                    (Display * dpy, XEvent *event)) {
    injector_init();
    int ret;
    do {
        ret = XNextEvent_ptr(dpy, event);
    } while (injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(XPeekEvent,
                                    int,
                                    (Display * dpy, XEvent *event)) {
    injector_init();
    int ret = XPeekEvent_ptr(dpy, event);
    while (injector_x11_handle_event(event)) {
        XNextEvent_ptr(dpy, event);
        ret = XPeekEvent_ptr(dpy, event);
    }
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(
    XWindowEvent, int, (Display * dpy, Window w, long mask, XEvent *event)) {
    injector_init();
    int ret;
    do {
        ret = XWindowEvent_ptr(dpy, w, mask, event);
    } while (injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(
    XCheckWindowEvent,
    Bool,
    (Display * dpy, Window w, long mask, XEvent *event)) {
    injector_init();
    Bool ret;
    do {
        ret = XCheckWindowEvent_ptr(dpy, w, mask, event);
    } while (ret && injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(XMaskEvent,
                                    int,
                                    (Display * dpy, long mask, XEvent *event)) {
    injector_init();
    int ret;
    do {
        ret = XMaskEvent_ptr(dpy, mask, event);
    } while (injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(XCheckMaskEvent,
                                    Bool,
                                    (Display * dpy, long mask, XEvent *event)) {
    injector_init();
    Bool ret;
    do {
        ret = XCheckMaskEvent_ptr(dpy, mask, event);
    } while (ret && injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(XCheckTypedEvent,
                                    Bool,
                                    (Display * dpy, int type, XEvent *event)) {
    injector_init();
    Bool ret;
    do {
        ret = XCheckTypedEvent_ptr(dpy, type, event);
    } while (ret && injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(
    XCheckTypedWindowEvent,
    Bool,
    (Display * dpy, Window w, int type, XEvent *event)) {
    injector_init();
    Bool ret;
    do {
        ret = XCheckTypedWindowEvent_ptr(dpy, w, type, event);
    } while (ret && injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(
    XIfEvent,
    int,
    (Display * dpy,
     XEvent *event,
     Bool (*predicate)(Display *, XEvent *, XPointer),
     XPointer arg)) {
    int ret;
    do {
        ret = XIfEvent_ptr(dpy, event, predicate, arg);
    } while (injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(
    XCheckIfEvent,
    Bool,
    (Display * dpy,
     XEvent *event,
     Bool (*predicate)(Display *, XEvent *, XPointer),
     XPointer arg)) {
    injector_init();
    Bool ret;
    do {
        ret = XCheckIfEvent_ptr(dpy, event, predicate, arg);
    } while (ret && injector_x11_handle_event(event));
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(
    XPeekIfEvent,
    int,
    (Display * dpy,
     XEvent *event,
     Bool (*predicate)(Display *, XEvent *, XPointer),
     XPointer arg)) {
    injector_init();
    int ret;
    while (injector_x11_handle_event(event)) {
        XIfEvent_ptr(dpy, event, predicate, arg);
        ret = XPeekIfEvent_ptr(dpy, event, predicate, arg);
    }
    return ret;
}

INJECTOR_OVERRIDE_SYMBOL_DEFINITION(XPending, int, (Display * dpy)) {
    injector_init();
    XEvent e;
    while (XCheckIfEvent(dpy, &e, injector_x11_check_event, NULL))
        ;
    return XPending_ptr(dpy);
}

void *injector_x_events_handle = NULL;
void  injector_load_x_events(const char *path) {
    if (injector_x_events_handle == NULL) {
        injector_x_events_handle = dlopen(path, RTLD_LOCAL | RTLD_LAZY);
    }
    if (!injector_x_events_handle) {
        printf("There was an error loading \"%s\"!", path);
        return;
    }

    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XPending);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XNextEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XPeekEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XWindowEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle,
                                      XCheckWindowEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XMaskEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle,
                                      XCheckMaskEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle,
                                      XCheckTypedEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle,
                                      XCheckTypedWindowEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XIfEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XCheckIfEvent);
    INJECTOR_LOAD_SYMBOL_USING_HANDLE(injector_x_events_handle, XPeekIfEvent);
}
