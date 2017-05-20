#pragma once

#include <X11/Xlib.h>
#include <piga-injector_export.h>
#include <stdbool.h>

#include <piga/event.h>

/**
 * The following events are solely for the overlay and are isolated from the
 * rest of the piga environment to facilitate API stability in user code. */

typedef struct piga_key_event_t {
    int  key;
    bool pressed;
} piga_key_event_t;
typedef struct piga_window_event_t {
    int width;
    int height;
} piga_window_event_t;
typedef struct piga_button_event_t {
    int button;
    int x;
    int y;
} piga_button_event_t;
typedef struct piga_motion_event_t {
    int x;
    int y;
} piga_motion_event_t;

PIGA_INJECTOR_NO_EXPORT Bool piga_x11_handle_event(XEvent *e);
PIGA_INJECTOR_NO_EXPORT int piga_x11_check_event(Display *display, XEvent *e, XPointer pointer);
