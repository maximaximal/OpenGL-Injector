#include "x_handling.h"
#include "../injectors.h"
#include "../lua_utils.h"

#include <stdio.h>

injector_window_event_t window_event;
injector_key_event_t    key_event;
injector_button_event_t button_event;
injector_motion_event_t motion_event;

#define SETUP_KEY_EVENT(ev, e, p)                                              \
    ev.key = XLookupKeysym(&e->xkey, 0);                                       \
    ev.pressed = p

#define SETUP_WINDOW_EVENT(ev, e)                                              \
    ev.width = 0;                                                              \
    ev.height = 0

#define SETUP_BUTTON_EVENT(ev, e)                                              \
    ev.button = 0;                                                             \
    ev.x = 0;                                                                  \
    ev.y = 0

#define SETUP_MOTION_EVENT(ev, e)                                              \
    ev.x = 0;                                                                  \
    ev.y = 0

Bool injector_x11_check_event(Display *display, XEvent *e, XPointer pointer) {
    if (!e) {
        return False;
    }
    Bool result = False;
    switch (e->type) {
    case KeyPress:
        SETUP_KEY_EVENT(key_event, e, true);
        result = injector_lua_call_bool_func_light_userdata(global_injector_handle->L,
                                               "shouldConsumeKeyEvent", &key_event);
        break;
    case KeyRelease:
        SETUP_KEY_EVENT(key_event, e, false);
        result = injector_lua_call_bool_func_light_userdata(global_injector_handle->L,
                                               "shouldConsumeKeyEvent", &key_event);
        break;
    case MotionNotify:
        SETUP_MOTION_EVENT(motion_event, e);
        result = injector_lua_call_bool_func_light_userdata(global_injector_handle->L,
                                               "shouldConsumeMotionEvent", &motion_event);
        break;
    case ButtonPress:
        SETUP_BUTTON_EVENT(button_event, e);
        result = injector_lua_call_bool_func_light_userdata(global_injector_handle->L,
                                               "shouldConsumeButtonEvent", &button_event);
        break;
    case ConfigureNotify:
        SETUP_WINDOW_EVENT(window_event, e);
        result = injector_lua_call_bool_func_light_userdata(global_injector_handle->L,
                                               "shouldConsumeWindowEvent", &window_event);
        break;
    }

    return result;
}
Bool injector_x11_handle_event(XEvent *e) {
    if (!e) {
        return false;
    }
    switch (e->type) {
    case KeyPress:
        SETUP_KEY_EVENT(key_event, e, true);
        injector_lua_call_void_func_light_userdata(global_injector_handle->L,
                                               "onKeyPress", &key_event);
        break;
    case KeyRelease:
        SETUP_KEY_EVENT(key_event, e, false);
        injector_lua_call_void_func_light_userdata(global_injector_handle->L,
                                               "onKeyRelease", &key_event);
        break;
    case MotionNotify:
        SETUP_MOTION_EVENT(motion_event, e);
        injector_lua_call_void_func_light_userdata(global_injector_handle->L,
                                               "onMotionNotify", &motion_event);
        break;
    case ButtonPress:
        SETUP_BUTTON_EVENT(button_event, e);
        injector_lua_call_void_func_light_userdata(global_injector_handle->L,
                                               "onButtonPress", &button_event);
        break;
    case ConfigureNotify:
        SETUP_WINDOW_EVENT(window_event, e);
        injector_lua_call_void_func_light_userdata(global_injector_handle->L,
                                               "onWindowEvent", &window_event);
        break;
    }

    return false;
}
