#include "x_events.h"

#include "x_handling.h"
#include "../injectors.h"
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

PIGA_OVERRIDE_SYMBOL_DEFINITION(XNextEvent,
                                int,
                                (Display * dpy, XEvent *event)) {
    piga_injector_init();
    int ret;
    do {
        ret = XNextEvent_ptr(dpy, event);
    } while (piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(XPeekEvent,
                                int,
                                (Display * dpy, XEvent *event)) {
    piga_injector_init();
    int ret = XPeekEvent_ptr(dpy, event);
    while(piga_x11_handle_event(event)) {
        XNextEvent_ptr(dpy, event);
        ret = XPeekEvent_ptr(dpy, event);
    }
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(
    XWindowEvent, int, (Display * dpy, Window w, long mask, XEvent *event)) {
    piga_injector_init();
    int ret;
    do {
        ret = XWindowEvent_ptr(dpy, w, mask, event);
    } while(piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(
    XCheckWindowEvent,
    Bool,
    (Display * dpy, Window w, long mask, XEvent *event)) {
    piga_injector_init();
    Bool ret;
    do {
        ret = XCheckWindowEvent_ptr(dpy, w, mask, event);
    } while(ret && piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(XMaskEvent,
                                int,
                                (Display * dpy, long mask, XEvent *event)) {
    piga_injector_init();
    int ret;
    do {
        ret = XMaskEvent_ptr(dpy, mask, event);
    } while(piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(XCheckMaskEvent,
                                Bool,
                                (Display * dpy, long mask, XEvent *event)) {
    piga_injector_init();
    Bool ret;
    do {
        ret = XCheckMaskEvent_ptr(dpy, mask, event);
    } while(ret && piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(XCheckTypedEvent,
                                Bool,
                                (Display * dpy, int type, XEvent *event)) {
    piga_injector_init();
    Bool ret;
    do {
        ret = XCheckTypedEvent_ptr(dpy, type, event);
    } while(ret && piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(
    XCheckTypedWindowEvent,
    Bool,
    (Display * dpy, Window w, int type, XEvent *event)) {
    piga_injector_init();
    Bool ret;
    do {
        ret = XCheckTypedWindowEvent_ptr(dpy, w, type, event);
    } while(ret && piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(
    XIfEvent,
    int,
    (Display * dpy,
     XEvent *event,
     Bool (*predicate)(Display *, XEvent *, XPointer),
     XPointer arg)) {
    int ret;
    do {
        ret = XIfEvent_ptr(dpy, event, predicate, arg);
    } while(piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(
    XCheckIfEvent,
    Bool,
    (Display * dpy,
     XEvent *event,
     Bool (*predicate)(Display *, XEvent *, XPointer),
     XPointer arg)) {
    piga_injector_init();
    Bool ret;
    do {
        ret = XCheckIfEvent_ptr(dpy, event, predicate, arg);
    } while(ret && piga_x11_handle_event(event));
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(
    XPeekIfEvent,
    int,
    (Display * dpy,
     XEvent *event,
     Bool (*predicate)(Display *, XEvent *, XPointer),
     XPointer arg)) {
    piga_injector_init();
    int ret;
    while(piga_x11_handle_event(event)) {
        XIfEvent_ptr(dpy, event, predicate, arg);
        ret = XPeekIfEvent_ptr(dpy, event, predicate, arg);
    }
    return ret;
}

PIGA_OVERRIDE_SYMBOL_DEFINITION(XPending, int, (Display * dpy)) {
    piga_injector_init();
    XEvent e;
    while(XCheckIfEvent(dpy, &e, piga_x11_check_event, NULL));
    return XPending_ptr(dpy);
}

void* piga_x_events_handle = NULL;
void piga_load_x_events(const char *path)
{
    if(piga_x_events_handle == NULL) {
        piga_x_events_handle = dlopen(path, RTLD_LOCAL | RTLD_LAZY);

    }
    if(!piga_x_events_handle) {
        printf("There was an error loading \"%s\"!", path);
        return;
    }

    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XPending);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XNextEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XPeekEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XWindowEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XCheckWindowEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XMaskEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XCheckMaskEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XCheckTypedEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XCheckTypedWindowEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XIfEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XCheckIfEvent);
    PIGA_LOAD_SYMBOL_USING_HANDLE(piga_x_events_handle, XPeekIfEvent);
}
