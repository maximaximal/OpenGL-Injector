#include "x_handling.h"
#include "../injectors.h"
#include "../lua_utils.h"

#include <stdio.h>

piga_window_event_t window_event;
piga_key_event_t    key_event;
piga_button_event_t button_event;
piga_motion_event_t motion_event;

bool piga_x11_handle_event(XEvent *e) {
    if (!e) {
        return false;
    }
    switch (e->type) {
    case KeyPress:
        key_event.key = XLookupKeysym(&e->xkey, 0);
        key_event.pressed = true;
        return piga_lua_call_bool_func_light_userdata(
            global_piga_injector_handle->L, "onKeyPress", &key_event);
        break;
    case KeyRelease:
        key_event.key = XLookupKeysym(&e->xkey, 0);
        key_event.pressed = false;
        return piga_lua_call_bool_func_light_userdata(
            global_piga_injector_handle->L, "onKeyRelease", &key_event);
        break;
    case MotionNotify:
        return piga_lua_call_bool_func_light_userdata(
            global_piga_injector_handle->L, "onMotionNotify", &motion_event);
        break;
    case ButtonPress:
        return piga_lua_call_bool_func_light_userdata(
            global_piga_injector_handle->L, "onButtonPress", &button_event);
        break;
    case ConfigureNotify:
        return piga_lua_call_bool_func_light_userdata(
            global_piga_injector_handle->L, "onWindowEvent", &window_event);
        break;
    }

    return false;
}
