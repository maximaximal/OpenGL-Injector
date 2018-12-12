#pragma once

#include <lua.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/inotify.h>

#include <cairo/cairo.h>

#include <injector_export.h>

enum injector_status_t {
    INJECTOR_INVALID_LUA_STATE = 1 << 0,
    INJECTOR_INVALID_SCRIPT_PATH = 1 << 1,
    INJECTOR_ERROR_IN_CONFIG_LUA = 1 << 2,
    INJECTOR_ERROR_IN_OVERLAY_LUA = 1 << 3,
};

struct injector_handle_t {
    const char *libGL_path;
    const char *libGLX_path;
    const char *libXlib_path;

    const char *vertex_shader;
    const char *fragment_shader;

    const char *path;
    char *      watched_path;

    unsigned int window_width;
    unsigned int window_height;

    int32_t status;

    lua_State *      L;
    cairo_surface_t *cairo_surface;
    cairo_t *        cairo_cr;
    int              cairo_stride;
    unsigned char *  cairo_data;

    bool draw_request;
    bool use_reloading;

    struct inotify_event *inotify_ev;
    int                   inotify_fd;
    int                   inotify_watch_fd;
    char *                inotify_ev_buf;
    size_t                inotify_ev_buf_size;
};

struct injector_handle_t *global_injector_handle;

INJECTOR_NO_EXPORT char *injector_combine_path(const char *p1, const char *p2);

INJECTOR_NO_EXPORT const char *injector_get_script_path();

INJECTOR_NO_EXPORT struct injector_handle_t *injector_init();

INJECTOR_NO_EXPORT void injector_check_inotify();

INJECTOR_NO_EXPORT void injector_refresh_lua();

INJECTOR_NO_EXPORT void injector_draw();

void INJECTOR_NO_EXPORT injector_exit();

INJECTOR_NO_EXPORT char *injector_read_file(const char *path, size_t *length);
