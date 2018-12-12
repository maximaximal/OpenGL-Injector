#pragma once

#include <X11/Xlib.h>
#include <injector_export.h>
#include <stdbool.h>

/**
 * The following events are solely for the overlay and are isolated from the
 * rest of the piga environment to facilitate API stability in user code. */

typedef struct injector_key_event_t {
    int  key;
    bool pressed;
} injector_key_event_t;
typedef struct injector_window_event_t {
    int width;
    int height;
} injector_window_event_t;
typedef struct injector_button_event_t {
    int button;
    int x;
    int y;
} injector_button_event_t;
typedef struct injector_motion_event_t {
    int x;
    int y;
} injector_motion_event_t;

INJECTOR_NO_EXPORT Bool injector_x11_handle_event(XEvent *e);
INJECTOR_NO_EXPORT int injector_x11_check_event(Display *display, XEvent *e, XPointer pointer);
